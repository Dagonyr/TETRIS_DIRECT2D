#pragma once
#include <unordered_map>
#include <string>
#include <d2d1.h>
#include <wrl/client.h>
class View;
class BrushCache
{
public:
    BrushCache(View* pview);
    void Register(const std::string& key, const D2D1::ColorF& color);
    void Register(const int key, const D2D1::ColorF& color);
    ID2D1SolidColorBrush* Get(const std::string& key) const;
    ID2D1SolidColorBrush* GetByInt(int key) const;
    void Clear();

private:
    ID2D1RenderTarget* pRT_ = nullptr;
    std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>> cache_;
    std::unordered_map<int, Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>> cache_ints_;
};