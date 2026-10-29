#pragma once
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <variant>
#include "Params.h"
#include "Controller.h"
#include <functional>
#include "funcs.h"
using Bindable = std::variant<const float*,float*, int*, double*>;
class Scope
{
public:
    explicit Scope(Lookup parent = nullptr) : parent_(std::move(parent)) {}
    Scope(const Scope&) = delete;             // лямбда внутри захватывает this
    Scope& operator=(const Scope&) = delete;

    void Define(std::string name, std::string expr)
    {
        expr_[std::move(name)] = std::move(expr);
    }

    bool Get(const std::string& name, float& out)
    {
        if (auto d = done_.find(name); d != done_.end()) { out = d->second; return true; }

        auto e = expr_.find(name);
        if (e == expr_.end())                       // не локальное имя: спросить родителя
            return parent_ && parent_(name, out);

        if (!inProgress_.insert(name).second) return false;   // цикл

        float v = 0.f;
        bool ok = countexpr(parcer(e->second),
            [this](const std::string& n, float& o) { return Get(n, o); }, v);
        inProgress_.erase(name);
        if (!ok) return false;

        done_[name] = v;
        out = v;
        return true;
    }

private:
    Lookup parent_;
    std::unordered_map<std::string, std::string> expr_;
    std::unordered_map<std::string, float> done_;
    std::unordered_set<std::string> inProgress_;
};
template <class Src, class Dst>
struct Field
{
    const char* name;   // имя, под которым поле видно в выражениях
    std::string Src::* in;   // указатель на член-строку
    float       Dst::* out;  // указатель на член-число
};

template <class Src, class Dst, size_t N>
bool Resolve(const Src& src, Dst& dst,
    const Field<Src, Dst>(&fields)[N], const Lookup& global)
{
    Scope scope(global);
    for (const auto& f : fields)
        scope.Define(f.name, src.*(f.in));          // поля превращаются в данные

    bool ok = true;
    for (const auto& f : fields)
        ok = scope.Get(f.name, dst.*(f.out)) && ok; // считаем все, даже после ошибки
    return ok;
}
class Bind
{
public:
    Bind() 
    {
        bindings["alpha_pole"] = &GameParams::alpha_pole;
        bindings["height_of_text_nxt_fig"] = &GameParams::height_of_text_nxt_fig;
    }
    std::unordered_map<std::string, Bindable> bindings;
    bool TryGet(const std::string& name,float& num)
    {
        auto it = bindings.find(name);
        if (it == bindings.end()) return false;
        num = std::visit([](auto* p) { return static_cast<float>(*p); }, it->second);
        return true;
    }

};

constexpr Field<strRect, floatRect> rectFields[] = {
    { "prc_width",  &strRect::prc_width,  &floatRect::prc_width  },
    { "prc_height",  &strRect::prc_height,  &floatRect::prc_height  },
    { "x_start",  &strRect::x_start,  &floatRect::x_start  },
    { "y_start",  &strRect::y_start,  &floatRect::y_start  }
};