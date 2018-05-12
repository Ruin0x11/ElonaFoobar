#pragma once
#include "thirdparty/sol2/sol.hpp"

namespace elona
{
namespace lua
{

extern std::unique_ptr<sol::state> sol;

void init();
void callback(const std::string&);
void load_mod(const std::string&);
void reload();

}
}
