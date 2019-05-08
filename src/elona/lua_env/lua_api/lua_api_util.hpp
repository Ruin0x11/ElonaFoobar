#pragma once
#include "lua_api_common.hpp"

namespace elona
{
namespace lua
{

/**
 * @luadoc
 *
 * Functions that don't fit in any other API.
 */
namespace LuaApiUtil
{

std::string from_shift_jis(const std::string&);

std::string to_shift_jis(const std::string&);

void bind(sol::table&);
}; // namespace LuaApiUtil

} // namespace lua
} // namespace elona
