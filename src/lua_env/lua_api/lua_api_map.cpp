#include "lua_api_map.hpp"
#include "../../lua_env/enums/enums.hpp"
#include "../../lua_env/interface.hpp"
#include "../../map.hpp"
#include "../../map_cell.hpp"

namespace elona
{
namespace lua
{

int Map::width()
{
    return map_data.width;
}

int Map::height()
{
    return map_data.height;
}

int Map::id()
{
    return game_data.current_map;
}

bool Map::is_overworld()
{
    return elona::map_data.atlas_number == 0;
}

bool Map::valid(const Position& position)
{
    return Map::valid_xy(position.x, position.y);
}

bool Map::valid_xy(int x, int y)
{
    if (Map::is_overworld())
    {
        return false;
    }
    if (x < 0 || y < 0 || x >= Map::width() || y >= Map::height())
    {
        return false;
    }

    return elona::cell_data.at(x, y).chip_id_actual != 0;
}

bool Map::is_solid(const Position& position)
{
    return Map::is_solid_xy(position.x, position.y);
}

bool Map::is_solid_xy(int x, int y)
{
    if (Map::is_overworld())
    {
        return true;
    }
    if (!Map::valid_xy(x, y))
    {
        return true;
    }

    return elona::chipm(7, elona::cell_data.at(x, y).chip_id_actual) & 4;
}

bool Map::is_blocked(const Position& position)
{
    return Map::is_blocked_xy(position.x, position.y);
}

bool Map::is_blocked_xy(int x, int y)
{
    if (Map::is_overworld())
    {
        return true;
    }
    if (!Map::valid_xy(x, y))
    {
        return true;
    }

    elona::cell_check(x, y);
    return elona::cellaccess != 0;
}

Position Map::bound_within(const Position& position)
{
    int x = clamp(position.x, 0, map_data.width - 1);
    int y = clamp(position.y, 0, map_data.height - 1);
    return Position{x, y};
}

Position Map::random_pos()
{
    return Map::bound_within(Position{elona::rnd(map_data.width - 1),
                                      elona::rnd(map_data.height - 1)});
}

int Map::generate_tile(const EnumString& type_name)
{
    TileKind type = LuaEnums::TileKindTable.ensure_from_string(type_name);
    return elona::cell_get_type(type);
}

int Map::get_tile(const Position& position)
{
    return Map::get_tile_xy(position.x, position.y);
}

int Map::get_tile_xy(int x, int y)
{
    if (Map::is_overworld())
    {
        return -1;
    }
    if (!Map::valid_xy(x, y))
    {
        return -1;
    }

    return elona::cell_data.at(x, y).chip_id_actual;
}

int Map::get_memory(const Position& position)
{
    return Map::get_memory_xy(position.x, position.y);
}

int Map::get_memory_xy(int x, int y)
{
    if (Map::is_overworld())
    {
        return -1;
    }
    if (!Map::valid_xy(x, y))
    {
        return -1;
    }

    return elona::cell_data.at(x, y).chip_id_memory;
}

sol::optional<LuaCharacterHandle> Map::get_chara(const Position& position)
{
    return Map::get_chara_xy(position.x, position.y);
}

sol::optional<LuaCharacterHandle> Map::get_chara_xy(int x, int y)
{
    if (!Map::valid_xy(x, y))
    {
        return sol::nullopt;
    }

    int index_plus_one = cell_data.at(x, y).chara_index_plus_one;

    if (index_plus_one == 0)
    {
        return sol::nullopt;
    }

    return lua::handle(cdata[index_plus_one - 1]);
}

void Map::set_tile(const Position& position, int type)
{
    Map::set_tile_xy(position.x, position.y, type);
}

void Map::set_tile_xy(int x, int y, int type)
{
    if (Map::is_overworld())
    {
        return;
    }
    if (!Map::valid_xy(x, y))
    {
        return;
    }

    elona::cell_data.at(x, y).chip_id_actual = type;
}

void Map::set_memory(const Position& position, int type)
{
    Map::set_memory_xy(position.x, position.y, type);
}

void Map::set_memory_xy(int x, int y, int type)
{
    if (Map::is_overworld())
    {
        return;
    }
    if (!Map::valid_xy(x, y))
    {
        return;
    }

    elona::cell_data.at(x, y).chip_id_memory = type;
}

void Map::bind(sol::table& api_table)
{
    LUA_API_BIND_FUNCTION(api_table, Map, width);
    LUA_API_BIND_FUNCTION(api_table, Map, height);
    LUA_API_BIND_FUNCTION(api_table, Map, id);
    LUA_API_BIND_FUNCTION(api_table, Map, is_overworld);
    api_table.set_function("valid", sol::overload(Map::valid, Map::valid_xy));
    api_table.set_function(
        "is_solid", sol::overload(Map::is_solid, Map::is_solid_xy));
    api_table.set_function(
        "is_blocked", sol::overload(Map::is_blocked, Map::is_blocked_xy));
    LUA_API_BIND_FUNCTION(api_table, Map, bound_within);
    LUA_API_BIND_FUNCTION(api_table, Map, random_pos);
    LUA_API_BIND_FUNCTION(api_table, Map, generate_tile);
    api_table.set_function(
        "get_tile", sol::overload(Map::get_tile, Map::get_tile_xy));
    api_table.set_function(
        "get_memory", sol::overload(Map::get_memory, Map::get_memory_xy));
    api_table.set_function(
        "get_chara", sol::overload(Map::get_chara, Map::get_chara_xy));
    api_table.set_function(
        "set_tile", sol::overload(Map::set_tile, Map::set_tile_xy));
    api_table.set_function(
        "set_memory", sol::overload(Map::set_memory, Map::set_memory_xy));
}

} // namespace lua
} // namespace elona
