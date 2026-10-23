#pragma once
#include <windows.h>
#include "view.h"
#include "model.h"
#include <d2d1.h>
#include "Params.h"
#include "Structs.h"
class Controller 
{
public:
	Controller(HINSTANCE hInstance, int nCmdShow);
	int StartGame();
private:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static void CALLBACK TimeProc(HWND hWnd, UINT msg, UINT_PTR idEvent, DWORD dwTime);
	void Release();
	std::string Choose_Color(int i);
	int Choose_ColorInt(int i);
	void IsGameRunning();
	void IsMenuRunning();
	void IsEndRunning();
	void UpdateFrame();


	void Transform_Figure(int code);
	void Move_Figure(int code);
	void Draw_Setka();
	void Draw_Cells();
	void Draw_Figure();
	void Draw_Text();
	void Draw_NextFigure();
	void IsCounterChanged();
	std::wstring GetCounter();


	GameLoopState gls_;
	Counter counter_;
	void CacheTextElements();
	HWND hWnd;
	Model model = Model();
	View* pview = nullptr;
	bool is_running = true;
	int setka_top;
	int setka_left;
	int size_of_cell;
	int Height;
	int Width;
	bool keys[256] = {false};
	bool prevKeys[256] = { false };
	bool is_counter_need_redraw = false;
	IDWriteTextLayout* Counterlayout = nullptr;
};