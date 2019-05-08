#include "lua_api_util.hpp"
#include <boost/locale.hpp>

namespace elona
{
namespace lua
{

/**
 * @luadoc
 *
 * Converts a string from Shift-JIS. Used for compatibility with vanilla Elona.
 */
std::string LuaApiUtil::from_shift_jis(const std::string& str)
{
    return boost::locale::conv::to_utf<char>(str, "shift-jis");
}

/**
 * @luadoc
 *
 * Converts a string to Shift-JIS. Used for compatibility with vanilla Elona.
 */
std::string LuaApiUtil::to_shift_jis(const std::string& str)
{
    return boost::locale::conv::from_utf(str, "shift-jis");
}

void LuaApiUtil::bind(sol::table& api_table)
{
    LUA_API_BIND_FUNCTION(api_table, LuaApiUtil, from_shift_jis);
    LUA_API_BIND_FUNCTION(api_table, LuaApiUtil, to_shift_jis);
}

} // namespace lua
} // namespace elona
