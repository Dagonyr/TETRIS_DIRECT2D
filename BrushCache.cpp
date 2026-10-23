#include "BrushCache.h"
#include "View.h"
BrushCache::BrushCache(View* pview) { pRT_ = pview->getpRT_(); }

void BrushCache::Register(const std::string& key, const D2D1::ColorF& color)
{
    if (!pRT_) return;
    ID2D1SolidColorBrush* pRaw = nullptr;
    HRESULT hr = pRT_->CreateSolidColorBrush(color, &pRaw);
    if (SUCCEEDED(hr))
    {
        cache_[key] = Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>(pRaw);
    }
}
void BrushCache::Register(const int key, const D2D1::ColorF& color)
{
    if (!pRT_) return;
    ID2D1SolidColorBrush* pRaw = nullptr;
    HRESULT hr = pRT_->CreateSolidColorBrush(color, &pRaw);
    if (SUCCEEDED(hr))
    {
        cache_ints_[key] = Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>(pRaw);
    }
}

ID2D1SolidColorBrush* BrushCache::Get(const std::string& key) const
{
    auto it = cache_.find(key);
    return (it != cache_.end()) ? it->second.Get() : nullptr;
}
ID2D1SolidColorBrush* BrushCache::GetByInt(int key) const
{
    auto it = cache_ints_.find(key);
    return (it != cache_ints_.end()) ? it->second.Get() : nullptr;
}
void BrushCache::Clear() { cache_.clear(); cache_ints_.clear(); }