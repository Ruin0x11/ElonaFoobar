#pragma once
#include "lua_api_common.hpp"

namespace elona
{
namespace lua
{

namespace Input
{
bool yes_no(const std::string&);

sol::optional<int> prompt_number(const std::string&, int);
sol::optional<int> prompt_number_with_initial(const std::string&, int, int);

sol::optional<std::string> prompt_text(const std::string&, bool);

sol::optional<int> prompt_choice(sol::table);

sol::optional<int> prompt_dialog(
    const std::string&,
    const std::string&,
    const std::string&,
    sol::table);
sol::optional<int> prompt_dialog_impress(
    const std::string&,
    const std::string&,
    const std::string&,
    int,
    int,
    sol::table);

sol::optional<int> prompt_dialog_with_chip(
    const std::string&,
    int,
    const std::string&,
    sol::table);
sol::optional<int> prompt_dialog_with_chip_impress(
    const std::string&,
    int,
    const std::string&,
    int,
    int,
    sol::table);


void bind(sol::table&);
}; // namespace Input

} // namespace lua
} // namespace elona
