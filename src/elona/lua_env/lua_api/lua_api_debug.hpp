#pragma once
#include <string>
#include "lua_api_common.hpp"

namespace elona
{
namespace lua
{


/**
 * @luadoc
 *
 * Functions for debugging scripts.
 */
namespace LuaApiDebug
{
void log(
    const std::string&,
    sol::optional<std::string>,
    sol::optional<EnumString>);

void report_error(const std::string&);


void bind(sol::table&);
} // namespace LuaApiDebug
} // namespace lua
} // namespace elona
