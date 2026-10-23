#pragma once
#include <unordered_map>
#include <string>
#include <dwrite.h>
#include <wrl/client.h> 
class View;
class TextFormatCache
{
public:

    TextFormatCache(View* pview);
    void Register(const std::string& key,
        const wchar_t* fontFamily,
        float fontSize,
        DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL);

    IDWriteTextFormat* Get(const std::string& key) const;
    void Clear();
private:
    IDWriteFactory* pFactory_ = nullptr;
    std::unordered_map<std::string, Microsoft::WRL::ComPtr<IDWriteTextFormat>> cache_;
};