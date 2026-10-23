#include <d2d1.h>
#include <dwrite.h>
#include <windows.h>
#include "View.h"
#include "TotalCache.h"
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib,"dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "uuid.lib")
View::View(HWND hWnd)
{
    INIT_FABRIC(hWnd);
 /*   CacheColors();*/
}
View::~View() 
{
    totalCache_->TextLayoutCache_.Clear();
    totalCache_->TextFormatCache_.Clear();
    totalCache_->BrushCache_.Clear();
    pRT_->Release();
    pD2DFactory_->Release();
    pDWriteFactory_->Release();
}
void View::INIT_FABRIC(HWND hwnd) 
{
    HRESULT hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        &pD2DFactory_
    );
    RECT rc;
    GetClientRect(hwnd, &rc);
   
    HRESULT hr1 = pD2DFactory_->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(
            hwnd,
            D2D1::SizeU(
                rc.right - rc.left,
                rc.bottom - rc.top), D2D1_PRESENT_OPTIONS_IMMEDIATELY
        ),
        &pRT_
    );
    pRT_->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
    INIT_res = hr1;
    rc_ = rc;



    DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&pDWriteFactory_)
    );
    IDWriteRenderingParams* pPixelParams = nullptr;
    pDWriteFactory_->CreateCustomRenderingParams(
        1.0f,                       // gamma
        0.0f,                       // enhancedContrast
        0.0f,                       // clearTypeLevel (0 = отключить субпиксель)
        DWRITE_PIXEL_GEOMETRY_FLAT, // геометрия пикселей
        DWRITE_RENDERING_MODE_ALIASED, // режим: чёткие пиксели без сглаживания
        &pPixelParams
    );

    if (pPixelParams) {
        pRT_->SetTextRenderingParams(pPixelParams);
        pPixelParams->Release();
    }


    HRESULT hrWic = CoCreateInstance(
        CLSID_WICImagingFactory,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_IWICImagingFactory,
        reinterpret_cast<void**>(&pWICFactory_)
    );
    if (FAILED(hrWic)) pWICFactory_ = nullptr;
    totalCache_ = std::make_unique<TotalCache>(this, L"assets\\ASSETS_INF.json");
}

ID2D1Bitmap* View::LoadBitmapFromFile(const wchar_t* uri)
{
    if (!pWICFactory_ || !pRT_) return nullptr;

    IWICBitmapDecoder* pDecoder = nullptr;
    IWICBitmapFrameDecode* pFrame = nullptr;
    IWICFormatConverter* pConverter = nullptr;
    ID2D1Bitmap* pBitmap = nullptr;

    HRESULT hr = pWICFactory_->CreateDecoderFromFilename(
        uri, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);

    if (SUCCEEDED(hr)) hr = pDecoder->GetFrame(0, &pFrame);

    if (SUCCEEDED(hr)) hr = pWICFactory_->CreateFormatConverter(&pConverter);
    if (SUCCEEDED(hr)) hr = pConverter->Initialize(
        pFrame,
        GUID_WICPixelFormat32bppPBGRA, 
        WICBitmapDitherTypeNone,
        nullptr,
        0.0f,
        WICBitmapPaletteTypeMedianCut
    );

    if (SUCCEEDED(hr)) hr = pRT_->CreateBitmapFromWicBitmap(pConverter, &pBitmap);

    if (pConverter) pConverter->Release();
    if (pFrame)    pFrame->Release();
    if (pDecoder)  pDecoder->Release();

    return SUCCEEDED(hr) ? pBitmap : nullptr;
}

void View::NotSafeDrawBitmap(ID2D1Bitmap* pBitmap, const D2D1_RECT_F& destRect, float opacity)
{
    if (!pBitmap || !pRT_) return;
    pRT_->DrawBitmap(
        pBitmap,
        &destRect,
        opacity,
        D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, // или LINEAR / NEAREST
        nullptr // sourceRect (nullptr = весь битмап)
    );
}
void View::NotSafeDrawRect(int left, int top, int right, int bottom) 
{
    if (!pRT_ || FAILED(INIT_res)) return;
    ID2D1SolidColorBrush* Brush = totalCache_->BrushCache_.GetByInt(Current_ColorInt_);
    if (!Brush) return;
    
        pRT_->FillRectangle(
            D2D1::RectF(left, top, right, bottom),
            Brush);
}
void View::NotSafeFillRects(const std::vector<D2D1_RECT_F>& rects)
{
    if (!pRT_ || FAILED(INIT_res) || rects.empty()) return;
    ID2D1SolidColorBrush* Brush = totalCache_->BrushCache_.GetByInt(Current_ColorInt_);
    if (!Brush) return;

    ID2D1PathGeometry* pGeometry = nullptr;
    if (FAILED(pD2DFactory_->CreatePathGeometry(&pGeometry))) return;

    ID2D1GeometrySink* pSink = nullptr;
    if (SUCCEEDED(pGeometry->Open(&pSink)))
    {
        for (const auto& r : rects)
        {
            pSink->BeginFigure(D2D1::Point2F(r.left, r.top), D2D1_FIGURE_BEGIN_FILLED);
            pSink->AddLine(D2D1::Point2F(r.right, r.top));
            pSink->AddLine(D2D1::Point2F(r.right, r.bottom));
            pSink->AddLine(D2D1::Point2F(r.left, r.bottom));
            pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
        }
        pSink->Close();
        pSink->Release();
        pRT_->FillGeometry(pGeometry, Brush); // один вызов вместо N
    }
    pGeometry->Release();
}
void View::NotSafeDrawLine(int x0, int y0, int x1, int y1, float strokeWidth) {
    // Защита от неинициализированного рендер-таргета
    if (!pRT_ || FAILED(INIT_res)) return;

    ID2D1SolidColorBrush* Brush = totalCache_->BrushCache_.GetByInt(Current_ColorInt_);

    if (!Brush) return;
        pRT_->DrawLine(
            D2D1::Point2F((FLOAT)x0, (FLOAT)y0),
            D2D1::Point2F((FLOAT)x1, (FLOAT)y1),
            Brush,
            strokeWidth
        );
}
void View::NotSafeDrawText(float pos_x, float pos_y, IDWriteTextLayout* pLayout)
{
    ID2D1SolidColorBrush* Brush = totalCache_->BrushCache_.GetByInt(Current_ColorInt_);
    if (!Brush) return;
    pRT_->DrawTextLayout(
        D2D1::Point2F(pos_x, pos_y),
        pLayout,
        Brush
    );
}

void View::NotSafeDrawText(float pos_x, float pos_y, std::string category,std::string name) 
{
    auto layout = totalCache_->TextLayoutCache_.Get(category, name);
    if (layout) NotSafeDrawText(pos_x, pos_y, layout);
}
IDWriteTextLayout* View::CreateLayout(const std::string& name,
    const std::wstring& text,
    float width,
    float height)
{
    auto pFormat = totalCache_->TextFormatCache_.Get(name);
    if (!pDWriteFactory_ || !pFormat) return nullptr;

        IDWriteTextLayout* pRaw = nullptr;
        const wchar_t* wtext = text.c_str();
        UINT32 textLen = static_cast<UINT32>(text.length());

        HRESULT hr = pDWriteFactory_->CreateTextLayout(
            wtext, textLen, pFormat, width, height, &pRaw);
        return pRaw;
}
void View::NotSafeClear() 
{
    pRT_->Clear(Colors::BLACK);
}
void View::Set_Color(std::string Color) 
{
    Current_Color_ = Color;
}
//void View::CacheTextFormats(std::string category, const wchar_t* fontFamily, float fontSize, std::string WEIGHT) 
//{
//    if (WEIGHT == "BOLD")
//    {
//        textFormats.Register(category, fontFamily, fontSize, DWRITE_FONT_WEIGHT_BOLD);
//    }
//    else 
//    {
//        textFormats.Register(category, fontFamily, fontSize);
//    }
//}
//void View::CacheTextElement(std::string category,std::string name,std::wstring text,float maxWidth,float maxHeight) 
//{
//    textLayout.Register(category, name, text, textFormats.Get(category), maxWidth, maxHeight);
//}
//void View::CacheColors() 
//{
//    brushCache.Register("WHITE_SOLID", Colors::WHITE);
//    brushCache.Register("RED_SOLID", Colors::RED);
//    brushCache.Register("BLUE_SOLID", Colors::BLUE);
//    brushCache.Register("BLACK_SOLID", Colors::BLACK);
//    brushCache.Register("WHITE_ORN_SOLID", Colors::WHITE_ORN);
//    brushCache.Register("IZ_SOLID", Colors::IZ);
//    brushCache.Register("FIOL_SOLID", Colors::FIOL);
//    brushCache.Register("MAR_SOLID", Colors::MAR);
//    brushCache.Register("GREEN_SOLID", Colors::GREEN);
//    brushCache.Register("YELLOW_SOLID", Colors::YELLOW);
//
//    brushCache.Register(ColorsInts::WHITE, Colors::WHITE);
//    brushCache.Register(ColorsInts::RED, Colors::RED);
//    brushCache.Register(ColorsInts::BLUE, Colors::BLUE);
//    brushCache.Register(ColorsInts::BLACK, Colors::BLACK);
//    brushCache.Register(ColorsInts::WHITE_ORN, Colors::WHITE_ORN);
//    brushCache.Register(ColorsInts::IZ, Colors::IZ);
//    brushCache.Register(ColorsInts::FIOL, Colors::FIOL);
//    brushCache.Register(ColorsInts::MAR, Colors::MAR);
//    brushCache.Register(ColorsInts::GREEN, Colors::GREEN);
//    brushCache.Register(ColorsInts::YELLOW, Colors::YELLOW);
//}
