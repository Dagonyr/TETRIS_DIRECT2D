#include "TextLayoutCache.h"
#include "View.h"
TextLayoutCache::TextLayoutCache(View* pview)
{
    pFactory_ = pview->getpIDWriteFactory();
}

void TextLayoutCache::Register(const std::string& category,
    const std::string& name,
    const std::wstring& text,
    IDWriteTextFormat* pFormat,
    float width,
    float height)
{
    if (!pFactory_ || !pFormat) return;

    IDWriteTextLayout* pRaw = nullptr;
    const wchar_t* wtext = text.c_str();
    UINT32 textLen = static_cast<UINT32>(text.length());

    HRESULT hr = pFactory_->CreateTextLayout(
        wtext, textLen, pFormat, width, height, &pRaw);

    if (SUCCEEDED(hr))
    {
        cache_[category][name] = Microsoft::WRL::ComPtr<IDWriteTextLayout>(pRaw);
    }
}
IDWriteTextLayout* TextLayoutCache::Get(const std::string& category, const std::string& name) const
{
    auto catIt = cache_.find(category);
    if (catIt != cache_.end())
    {
        auto itemIt = catIt->second.find(name);
        if (itemIt != catIt->second.end())
        {
            return itemIt->second.Get();
        }
    }
    return nullptr;
}
std::unordered_map<std::string, Microsoft::WRL::ComPtr<IDWriteTextLayout>> TextLayoutCache::Get(const std::string& category)
{
    return cache_[category];
}

void TextLayoutCache::Clear() { cache_.clear(); }