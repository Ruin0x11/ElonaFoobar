#pragma once
#include "thirdparty/sol2/sol.hpp"
#include <map>

namespace elona
{
namespace lua
{

extern std::unique_ptr<sol::state> sol;

void init();
void callback(const std::string&);
void callback(const std::string&, const std::map<std::string, int>);
void load_mod(const std::string&);
void on_chara_creation(int id);
void on_chara_removal(int id);
void reload();

}
}
