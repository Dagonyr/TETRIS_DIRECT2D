#include "View.h"
#include "Model.h"
#include "Controller.h"
#include <Windows.h>
#include <shellscalingapi.h> 
#include <d2d1.h>
#include <chrono>
#include <thread>
#include "TotalCache.h"
int Controller::StartGame() 
{
	//SetTimer(hWnd, 1, 500, TimeProc);
	//SetTimer(hWnd, 2, 1000, TimeProc);
	MSG msg = {};
	ShowWindow(hWnd,SW_SHOW);
	UpdateWindow(hWnd);
	model.generate();
	UpdateFrame();
	while (is_running)
	{
		if (model.is_lose) 
		{
			GameParams::game_state = 2;
			UpdateFrame();
			msg.message = WM_DESTROY;
			break;
		}
		while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT) break;
		switch (GameParams::game_state) 
		{
		case 0:
			IsMenuRunning();
			break;
		case 1:
			IsGameRunning();
			break;
		case 2:
			IsEndRunning();
			break;
		}
	}
	return (int)(msg.wParam);
}
void Controller::IsMenuRunning() {}
void Controller::IsGameRunning()
{

	bool kLeft = (GetAsyncKeyState(0x41) & 0x8000) != 0;
	bool kRight = (GetAsyncKeyState(0x44) & 0x8000) != 0;
	bool kDown = (GetAsyncKeyState(0x53) & 0x8000) != 0;
	bool kLeftRot = (GetAsyncKeyState(0x25) & 0x8000) != 0;
	auto now1 = std::chrono::high_resolution_clock::now();
	gls_.frameTime = std::chrono::duration<double>(now1 - gls_.prevTime).count();
	gls_.prevTime = now1;
	gls_.accumulator += gls_.frameTime;
	if (gls_.accumulator >= GameParams::input_lag) // одиночные действия
		if (kLeftRot && !prevKeys[0x25]) {
			model.left_Transform();
		}

	if ((kLeft && prevKeys[0x41]) || (kRight && prevKeys[0x44]) || (kDown && prevKeys[0x53]))
	{
		gls_.dasTimer += gls_.frameTime;
	}
	else { gls_.dasTimer = 0.0f; gls_.delayTimer = 0.0f; }


	if (gls_.dasTimer < GameParams::DAS_DELAY)
	{
		if (kLeft && !prevKeys[0x41])   model.left_Move();
		if (kRight && !prevKeys[0x44])  model.right_Move();
		if (kDown && !prevKeys[0x53])   model.Low_Move();
	}
	else
	{
		gls_.delayTimer += gls_.frameTime;
		if (gls_.delayTimer >= GameParams::DAS_RATE)
		{
			if (kLeft)  model.left_Move();
			if (kRight)  model.right_Move();
			if (kDown)             model.Low_Move();
		}
	}

	prevKeys[0x41] = kLeft; // Запоминаем для следующего кадра
	prevKeys[0x44] = kRight;
	prevKeys[0x53] = kDown;
	prevKeys[0x25] = kLeftRot;
	
	if (gls_.accumulator >= GameParams::speed_of_fall)
	{
		IsCounterChanged();
		model.Low_Move();
		gls_.accumulator = 0.0f;
	}
	UpdateFrame();
	auto frameEnd = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - gls_.frameStart);
	if (elapsed < GameParams::targetFrameTime)
		std::this_thread::sleep_for(GameParams::targetFrameTime - elapsed - std::chrono::milliseconds(1));
	gls_.frameStart = std::chrono::high_resolution_clock::now();
}
void Controller::IsEndRunning() {}
void Controller::UpdateFrame() 
{
	switch (GameParams::game_state) 
	{
	case 0:
		(*pview).getpRT_()->BeginDraw();
		(*pview).NotSafeClear();
		(*pview).getpRT_()->EndDraw();
		break;
	case 1:
		(*pview).getpRT_()->BeginDraw();
		(*pview).NotSafeClear();
		Draw_Cells();
		Draw_Figure();
		Draw_Setka();
		Draw_Text();
		Draw_NextFigure();
		(*pview).getpRT_()->EndDraw();
		break;
	}
}
void Controller::Move_Figure(int code) 
{
	switch (code) 
	{
	case 0x53:
		model.Low_Move();
		break;
	case 0x41:
		model.left_Move();
		break;
	case 0x44:
		model.right_Move();
		break;
	}
}
void Controller::Transform_Figure(int code)
{
	switch (code)
	{
	case 0x25:
		model.left_Transform();
		break;
	case 0x27:
		model.left_Transform();
		break;
	}
}
void Controller::Draw_Setka() 
{
	int Inter_Width = GameParams::alpha_pole * Width;
	(*pview).Current_ColorInt_ = ColorsInts::WHITE;
	for (int i = 0; i <= model.w; i++) (*pview).NotSafeDrawLine(
		setka_left+i*(size_of_cell),
		setka_top,
		setka_left+i * (size_of_cell),
		setka_top + (size_of_cell)*model.h); //VERTICAL
	for (int i = 0; i <= model.h; i++) (*pview).NotSafeDrawLine(
		setka_left,
		setka_top+i*(size_of_cell),
		setka_left+model.w * (size_of_cell),
		setka_top+i*(size_of_cell)); //HORIZONTAL
	(*pview).Current_ColorInt_ = ColorsInts::WHITE;
	(*pview).NotSafeDrawLine(Inter_Width,0,Inter_Width,Height);
	this->setka_left = setka_left;
	this->setka_top = setka_top;
}

void Controller::Draw_Cells()
{
	std::unordered_map<int, std::vector<D2D1_RECT_F>> byColor;
	for (int i = 0; i < model.w; i++)
		for (int j = 0; j < model.h; j++)
		{
			int color = model.pole[i][j + model.generate_h];
			byColor[color].push_back(D2D1::RectF(
				(float)(setka_left + i * size_of_cell),
				(float)(setka_top + j * size_of_cell),
				(float)(setka_left + (i + 1) * size_of_cell),
				(float)(setka_top + (j + 1) * size_of_cell)));
		}
	for (auto& [color, rects] : byColor)
	{
		(*pview).Current_ColorInt_ = Choose_ColorInt(color);
		(*pview).NotSafeFillRects(rects); 
	}
}
void Controller::Draw_Figure() 
{
	int x;
	int y;
	(*pview).Current_ColorInt_ = Choose_ColorInt(model.cur_el);
	for (int i = 0; i < model.mx_elems; i++) 
	{
		x = model.cur_figure[i].x;
		y = model.cur_figure[i].y - model.generate_h;
		if (y >= 0)
		{
			(*pview).NotSafeDrawRect(
				setka_left + x * size_of_cell,
				setka_top + y * size_of_cell,
				setka_left + (x + 1) * size_of_cell,
				setka_top + (y + 1) * size_of_cell);
		}
	}
}
void Controller::Draw_Text() 
{
	switch (GameParams::game_state) 
	{
	case 1:
		std::string str = "Counter";
		if (is_counter_need_redraw || !Counterlayout) Counterlayout = (*pview).CreateLayout(str, L"СЧЕТ:" + GetCounter(),
			(1 - GameParams::alpha_pole) * Width,
			GameParams::height_count * Height);
		(*pview).NotSafeDrawText(GameParams::pos_of_count[0] * Width, GameParams::pos_of_count[1] * Height,
			Counterlayout);
			is_counter_need_redraw = false;
		(*pview).NotSafeDrawText(GameParams::pos_of_text_nxt_fig[0] * Width, GameParams::pos_of_text_nxt_fig[1] * Height, str, "Count_Text_Next_Figure");
	}
}
void Controller::Draw_NextFigure() 
{
	point* buf_fig = new point[4];
	model.Get_Start_Pos(buf_fig);
	int sz_on_pix_witdh = (1 - GameParams::pos_of_nxt_fig[0] - 0.05 * 2)*Width/4;
	int sz_on_pix_height = GameParams::height_of_nxt_fig * Height / 4;
	int sz_on_pix = min(sz_on_pix_witdh, sz_on_pix_height);
	int dx = (GameParams::pos_of_nxt_fig[0] + 0.1) * Width;
	int dy = (GameParams::pos_of_nxt_fig[1]) * Height;
	for (int i = 0; i < 4; i++) 
	{
		int x0 = dx + sz_on_pix * buf_fig[i].x;
		int y0 = dy + sz_on_pix * buf_fig[i].y;
		int x1 = dx + sz_on_pix * (buf_fig[i].x + 1);
		int y1 = dy + sz_on_pix * (buf_fig[i].y + 1);
		(*pview).Current_ColorInt_ = Choose_ColorInt((model.cur_el + 1) % 7);
		(*pview).NotSafeDrawRect(x0,y0,x1,y1);
		(*pview).Current_ColorInt_ = ColorsInts::WHITE;
		(*pview).NotSafeDrawLine(x0, y0, x1, y0);
		(*pview).NotSafeDrawLine(x0, y1, x1, y1);
		(*pview).NotSafeDrawLine(x0, y0, x0, y1);
		(*pview).NotSafeDrawLine(x1, y0, x1, y1);

	}
}
void Controller::IsCounterChanged() 
{
	if (model.counter_.old_counter != model.counter_.counter) 
	{ 
		is_counter_need_redraw = true; 
		if (Counterlayout) Counterlayout->Release();

	}
}
//void Controller::CacheTextElements() 
//{
//	std::string cur_str = "Counter";
//	(*pview).CacheTextFormats(cur_str, L"PerfectDOSVGA437", 64.0f, "BOLD");
//	(*pview).CacheTextElement("Counter", "Count_Text_Next_Figure", L"Следующая фигура:", (1 - GameParams::alpha_pole) * Width, GameParams::height_of_text_nxt_fig * Height);
//}
std::wstring Controller::GetCounter()
{
	std::wstring str_counter = std::to_wstring(model.counter_.counter);
	int delta = 4 - str_counter.length();
	if (delta >=0) 
	{
		for (int i = 0; i < delta; i++) str_counter = L"0" + str_counter;
	}
	return str_counter;
}
void Controller::Release() 
{
	delete pview;
}
int Controller::Choose_ColorInt(int i)
{
	int Current_Color_ = ColorsInts::BLACK;
	switch (i)
	{
	case 0:
	{
		Current_Color_ = ColorsInts::RED;
		break;
	}
	case 1:
	{
		Current_Color_ = ColorsInts::WHITE_ORN;
		break;
	}
	case 2:
	{
		Current_Color_ = ColorsInts::BLUE;
		break;
	}
	case 3:
	{
		Current_Color_ = ColorsInts::FIOL;
		break;
	}
	case 4:
	{
		Current_Color_ = ColorsInts::IZ;
		break;
	}
	case 5:
	{
		Current_Color_ = ColorsInts::YELLOW;
		break;
	}
	case 6:
	{
		Current_Color_ = ColorsInts::MAR;
		break;
	}
	}
	return Current_Color_;
}
Controller::Controller(HINSTANCE hInstance, int nCmdShow) 
{
	HMONITOR hMon = MonitorFromWindow(nullptr, MONITOR_DEFAULTTOPRIMARY);
	MONITORINFO mi = { sizeof(mi) };
	GetMonitorInfo(hMon, &mi);

	int fullW = mi.rcMonitor.right - mi.rcMonitor.left;
	int fullH = mi.rcMonitor.bottom - mi.rcMonitor.top;
    // the handle for the window, filled by a function
    HWND Wnd;
    // this struct holds information for the window class
    WNDCLASSEX wc;

    // clear out the window class for use
    ZeroMemory(&wc, sizeof(WNDCLASSEX));


    // fill in the struct with the needed information
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"WindowClass";

    // register the window class
    RegisterClassEx(&wc);
    // create the window and use the result as the handle
    Wnd = CreateWindowEx(NULL,
        L"WindowClass",    // name of the window class
        L"Tetris",   // title of the window
        WS_POPUP | WS_VISIBLE,    // window style
        mi.rcMonitor.left,    // x-position of the window
		mi.rcMonitor.top,    // y-position of the window
        fullW,    // width of the window
        fullH,    // height of the window
        NULL,    // we have no parent window, NULL
        NULL,    // we aren't using menus, NULL
        hInstance,    // application handle
        this);    // used with multiple windows, NULL
    hWnd = Wnd;
	pview = new View(hWnd);
	RECT rc = (*pview).getRect();
	Height = rc.bottom - rc.top;
	Width = GameParams::alpha_pole*(rc.right - rc.left);
	size_of_cell = (1 - 2 * model.border_hight) * Height / model.h +1;
	setka_top = rc.top + (Height -model.h*size_of_cell)/2;
	setka_left = rc.left + (Width - model.w * size_of_cell) / 2;
	Width = (rc.right - rc.left);
}
LRESULT CALLBACK Controller::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY: {
		Controller* pcontroller = reinterpret_cast<Controller*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		View* pview = pcontroller->pview;
		if (pcontroller)
		{
			pcontroller->Release();
			pcontroller->is_running = false;
		}
		break;
	}
	case WM_KEYDOWN:
	{

		switch (wParam) {
		case VK_ESCAPE:
		{
			PostQuitMessage(0);
			return 0;
		}
		default:
		{
			Controller* pcontroller = reinterpret_cast<Controller*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			if (pcontroller && wParam < 256) pcontroller->keys[wParam] = true;
		}
		}
		break;
	}
	//case WM_KEYUP:
	//{
	//	Controller* pcontroller = reinterpret_cast<Controller*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	//	if (wParam < 256) pcontroller->keys[wParam] = false; break;
	//}
	case WM_NCCREATE:
	{

		CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
		Controller* pView = reinterpret_cast<Controller*>(cs->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pView));
		break;
	}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
void CALLBACK Controller::TimeProc(HWND hWnd, UINT msg, UINT_PTR idEvent, DWORD dwTime)
{
	if (idEvent == 1)
	{
	}
	if (idEvent == 2)
	{
	}
}