#include "TextFormatCache.h"
#include "View.h"

TextFormatCache::TextFormatCache(View* pview)
{
    pFactory_ = pview->getpIDWriteFactory();
}
void TextFormatCache::Register(const std::string& key,
    const wchar_t* fontFamily,
    float fontSize,
    DWRITE_FONT_WEIGHT weight,
    DWRITE_FONT_STYLE style)
{
    if (!pFactory_) return;

    IDWriteTextFormat* pRaw = nullptr;
    HRESULT hr = pFactory_->CreateTextFormat(
        fontFamily, nullptr, weight, style, DWRITE_FONT_STRETCH_NORMAL,
        fontSize, L"en-us", &pRaw);

    if (SUCCEEDED(hr)) {
        cache_[key] = Microsoft::WRL::ComPtr<IDWriteTextFormat>(pRaw);
    }
}
IDWriteTextFormat* TextFormatCache::Get(const std::string& key) const
{
    auto it = cache_.find(key);
    if (it != cache_.end()) return it->second.Get();
    return nullptr;
}
void TextFormatCache::Clear()
{
    cache_.clear();
}