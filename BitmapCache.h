#pragma once
#include <unordered_map>
#include <string>
#include <wrl/client.h>
#include <d2d1.h>
#include <wincodec.h> 
class View;
class BitmapCache
{
public:

    BitmapCache(View* pview);
    void Register(const wchar_t*& key);
    ID2D1Bitmap* Get(const wchar_t*& key) const;
    void Clear();
private:
    View* pview_ = nullptr;
    std::unordered_map<const wchar_t*, Microsoft::WRL::ComPtr<ID2D1Bitmap>> cache_;
};