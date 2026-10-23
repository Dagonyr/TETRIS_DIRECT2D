#include "TotalCache.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include "Structs.h"
#include "View.h"
#include "funcs.h"
#include "Bindings.h"
using json = nlohmann::json;
void TotalCache::ParseAssets()
{
    std::ifstream f(link_to_assets);
    if (!f.is_open()) return;
    json j = json::parse(f, nullptr, false);
    if (j.is_discarded()) return;

    if (j.contains("SolidBrushs") && j["SolidBrushs"].is_array())
    {
        auto solidBrushs = j["SolidBrushs"].get<std::vector<SolidBrush>>();
        for (const auto& it : solidBrushs)
            BrushCache_.Register(it.key, FromHexToColorF(it.color));
    }
    if (j.contains("TextFormats") && j["TextFormats"].is_array())
    {
        auto textFormats = j["TextFormats"].get<std::vector<TextFormat>>();
        for (const auto& it : textFormats)
        {
            std::wstring wstr_it(it.fontFamily.begin(), it.fontFamily.end());
            TextFormatCache_.Register(it.key, wstr_it.c_str(), it.fontSize);
        }
    }
    //if (j.contains("TextLayouts") && j["TextLayouts"].is_array())
    //{
    //    auto textLayouts = j["TextLayouts"].get<std::vector<TextLayout>>();
    //    for (const auto& it : textLayouts)
    //    {

    //        TextLayoutCache_.Register(it.category,it.name,Utf8ToWide(it.text),TextFormatCache_.Get(it.category),)
    //    }
    //}
   // auto bitmapPaths = j["BitmapPaths"].get<std::vector<BitmapPath>>();


}
