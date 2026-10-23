#pragma once
#include <unordered_map>
#include <string>
#include <dwrite.h>
#include <wrl/client.h>
class View;
class TextLayoutCache
{
public:
    TextLayoutCache(View* pview);
    void Register(const std::string& category,
        const std::string& name,
        const std::wstring& text,
        IDWriteTextFormat* pFormat,
        float width,
        float height);
    IDWriteTextLayout* Get(const std::string& category, const std::string& name) const;
    std::unordered_map<std::string, Microsoft::WRL::ComPtr<IDWriteTextLayout>> Get(const std::string& category);
    void Clear();

private:
    IDWriteFactory* pFactory_ = nullptr;
    std::unordered_map<std::string, std::unordered_map<std::string, Microsoft::WRL::ComPtr<IDWriteTextLayout>>> cache_;
};