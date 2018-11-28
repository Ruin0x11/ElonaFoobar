#pragma once
#include "lua_api_common.hpp"

namespace elona
{
namespace lua
{

namespace Macro
{
void enqueue(const std::string&);
void enqueue_table(sol::table);

void clear_queue();


void bind(sol::table&);
}; // namespace Macro

} // namespace lua
} // namespace elona
