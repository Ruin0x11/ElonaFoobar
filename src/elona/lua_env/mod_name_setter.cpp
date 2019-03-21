#include "mod_name_setter.hpp"
#include "lua_env.hpp"

namespace elona
{
namespace lua
{

static const char* loading_mod_name_key = "_LOADING_MOD_NAME";

ModNameSetter::ModNameSetter(const std::string& mod_name)
{
    lua::lua->get_state()->registry().set(loading_mod_name_key, mod_name);
}

ModNameSetter::~ModNameSetter()
{
    lua::lua->get_state()->registry().set(loading_mod_name_key, sol::lua_nil);
}

sol::optional<std::string> loading_mod_name()
{
    return lua::lua->get_state()->registry()[loading_mod_name_key];
}

} // namespace lua
} // namespace elona
