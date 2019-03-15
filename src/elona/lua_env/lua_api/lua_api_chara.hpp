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
 * Functions for working with characters.
 */
namespace LuaApiChara
{
bool is_alive(LuaCharacterHandle);

bool is_player(LuaCharacterHandle);

bool is_ally(LuaCharacterHandle);

int count();

sol::optional<LuaCharacterHandle> player();

sol::optional<LuaCharacterHandle> create_random(const Position&);
sol::optional<LuaCharacterHandle> create_random_xy(int, int);
sol::optional<LuaCharacterHandle> create_from_id(
    const Position&,
    const std::string&);
sol::optional<LuaCharacterHandle>
create_from_id_xy(int, int, const std::string&);

sol::optional<LuaCharacterHandle> generate_from_map();
sol::optional<LuaCharacterHandle> generate_from_map_pos(const Position&);
sol::optional<LuaCharacterHandle> generate_from_map_xy(int, int);

int kill_count(const std::string&);

sol::optional<LuaCharacterHandle> find(const std::string&, const EnumString&);

bool can_recruit_allies();

void bind(sol::table&);
}; // namespace LuaApiChara

} // namespace lua
} // namespace elona
