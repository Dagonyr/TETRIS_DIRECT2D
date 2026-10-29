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
    Bind bind = Bind();
    Lookup global = [&bind](const std::string& n, float& o) { return bind.TryGet(n, o); };
    Scope scope = Scope(global);
    std::ifstream f(link_to_assets);
    if (!f.is_open()) return;
    json j;
    try {
        j = json::parse(f); // Включаем исключения для диагностики
    }
    catch (const json::parse_error& e) {
        std::string err = "JSON parse error:\n" + std::string(e.what()) +
            "\nПозиция в файле: байт " + std::to_string(e.byte);
        MessageBoxA(NULL, err.c_str(), "JSON Error", MB_OK);
        return;
    }

    if (j.is_discarded()) {
        MessageBoxA(NULL, "JSON discarded (неизвестная ошибка парсинга)", "Error", MB_OK);
        return;
    }
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
    if (j.contains("TextLayouts") && j["TextLayouts"].is_array())
    {
        auto textLayouts = j["TextLayouts"].get<std::vector<TextLayout>>();
        for (const auto& it : textLayouts)
        {
            float height;
            float width;
            bool fl1 = countexpr(it.height, global,height);
            bool fl2 = countexpr(it.width, global, width);
            if (!fl1 || !fl2) continue;
            TextLayoutCache_.Register(it.category, it.name, Utf8ToWide(it.text), TextFormatCache_.Get(it.category), width, height);
        }
    }
    //auto bitmapPaths = j["BitmapPaths"].get<std::vector<BitmapPath>>();


}
