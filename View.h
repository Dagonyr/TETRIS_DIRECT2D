#pragma once
#include <d2d1.h>
#include <windows.h>
#include <wincodec.h> 
#include "TotalCache.h"
#include <memory>
namespace Colors {
    inline const D2D1::ColorF WHITE = { 1.0f, 1.0f, 1.0f, 1.0f }; 
    inline const D2D1::ColorF BLACK = { 0.0f, 0.0f, 0.0f, 1.0f }; 
    inline const D2D1::ColorF RED = { 1.0f, 0.0f, 0.0f, 1.0f };
    inline const D2D1::ColorF GREEN = { 0.0f, 1.0f, 0.0f, 1.0f };
    inline const D2D1::ColorF BLUE = { 0.0f, 0.0f, 1.0f, 1.0f };
    inline const D2D1::ColorF YELLOW = { 1.0f, 1.0f, 0.0f, 1.0f };
    inline const D2D1::ColorF FIOL = { 120.0f/255.0f, 81.0f/255.0f, 169.0f/255.0f };
    inline const D2D1::ColorF IZ = { 80.0f / 255.0f, 200.0f / 255.0f, 120.0f / 255.0f };
    inline const D2D1::ColorF WHITE_ORN = { 250.0f / 223.0f, 173.0f / 255.0f, 250.0f / 255.0f };
    inline const D2D1::ColorF MAR = { 102.0f / 255.0f, 0.0f / 255.0f, 51.0f / 255.0f };
}
namespace ColorsInts 
{
    inline const int WHITE = 0;
    inline const int BLACK = 1;
    inline const int RED = 2;
    inline const int GREEN = 3;
    inline const int BLUE = 4;
    inline const int YELLOW = 5;
    inline const int FIOL = 6;
    inline const int IZ = 7;
    inline const int WHITE_ORN = 8;
    inline const int MAR = 9;
}
class View
{
public:
    ID2D1HwndRenderTarget* getpRT_() const { return pRT_; }
    IDWriteFactory* getpIDWriteFactory() const { return pDWriteFactory_; }
    ID2D1Factory* getpID2D1Factory() const { return pD2DFactory_; }
    IWICImagingFactory* getpIWICImagingFactory() const { return pWICFactory_; }
    std::string Current_Color_ = "BLACK_SOLID";
    int Current_ColorInt_ = ColorsInts::BLACK;
	void Set_Color(std::string Color);
    void NotSafeDrawRect(int x0, int y0, int x1, int y1);
    void NotSafeDrawLine(int x0, int y0, int x1, int y1, float strokeWidth = 1.0f);
    void NotSafeClear();
    void NotSafeDrawText(float pos_x, float pos_y, IDWriteTextLayout* Layout);
    void NotSafeDrawText(float pos_x, float pos_y, std::string category, std::string name);
    void NotSafeDrawBitmap(ID2D1Bitmap* pBitmap, const D2D1_RECT_F& destRect, float opacity = 1);
    void NotSafeFillRects(const std::vector<D2D1_RECT_F>& rects);
    IDWriteTextLayout* CreateLayout(const std::string& name,const std::wstring& text,float width,float height);
    ID2D1Bitmap* LoadBitmapFromFile(const wchar_t* uri);
    ~View();
    RECT rc_;
    View(HWND hWnd);
private:
void INIT_FABRIC(HWND hwnd);
ID2D1HwndRenderTarget* pRT_ = nullptr;
IDWriteFactory* pDWriteFactory_ = nullptr;
ID2D1Factory* pD2DFactory_ = nullptr;
IWICImagingFactory* pWICFactory_ = nullptr;
std::unique_ptr<TotalCache> totalCache_;
HRESULT INIT_res;
};

