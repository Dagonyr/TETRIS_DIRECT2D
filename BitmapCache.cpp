#include "BitmapCache.h"
#include "View.h"
BitmapCache::BitmapCache(View* pview)
{
    pview_ = pview;
}

void BitmapCache::Register(const std::wstring& key)
{
    ID2D1Bitmap* pBitmap = (*pview_).LoadBitmapFromFile(key.c_str());
    if (pBitmap) cache_[key] = Microsoft::WRL::ComPtr<ID2D1Bitmap>(pBitmap);
}

ID2D1Bitmap* BitmapCache::Get(const std::wstring& key) const
{
    auto it = cache_.find(key);
    if (it != cache_.end()) return it->second.Get();
    return nullptr;
}
void BitmapCache::Clear()
{
    cache_.clear();
}