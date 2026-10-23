#pragma once
#include <unordered_map>
#include <string>
#include <variant>
#include "Params.h"
#include "Controller.h"
using Bindable = std::variant<float*, int*, double*>;
class Bind 
{
public:
    //Bind() 
    //{
    //    bindings["alpha_pole"] = 
    //}
    std::unordered_map<std::string, Bindable> bindings;
    float GetValue(const std::string& name)
    {
        auto it = bindings.find(name);
        if (it == bindings.end()) return 0.f;
        return std::visit([](auto* p) { return static_cast<float>(*p); }, it->second);
    }

};