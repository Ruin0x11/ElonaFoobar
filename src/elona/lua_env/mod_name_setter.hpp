#pragma once
#include "../../thirdparty/sol2/sol.hpp"

namespace elona
{
namespace lua
{

class ModNameSetter
{
public:
    ModNameSetter(const std::string& mod_name);
    ~ModNameSetter();
};

sol::optional<std::string> loading_mod_name();

} // namespace lua
} // namespace elona
