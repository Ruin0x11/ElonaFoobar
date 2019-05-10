#pragma once
#include "lua_api_common.hpp"

namespace elona
{
namespace lua
{

/**
 * @luadoc
 *
 * Functions for manipulating the player's user interface.
 */
namespace LuaApiGUI
{
void txt(const std::string&, sol::variadic_args);

void txt_colored(const std::string&, const EnumString&, sol::variadic_args);

void txtnew();

void play_sound(const std::string&);

void show_journal_update_message();

void fade_out();


void bind(sol::table&);
}; // namespace LuaApiGUI

} // namespace lua
} // namespace elona
