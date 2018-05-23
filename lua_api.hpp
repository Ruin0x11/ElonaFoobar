#pragma once

#include "character.hpp"
#include "filesystem.hpp"
#include "item.hpp"
#include "lua.hpp"

namespace elona
{
namespace lua
{

class lua_env;
struct mod_info;

class api_manager
{
public:
    static void bind(lua_env&, mod_info&);
    static void bind(lua_env&);
public:
    explicit api_manager(lua_env*);
    void load_core(lua_env&, const fs::path&);
    sol::optional<sol::table> try_find_api(const std::string&, const std::string&);
    sol::table get_api_table();
private:
    sol::environment api_env;
    lua_env* lua;
};

}
}
