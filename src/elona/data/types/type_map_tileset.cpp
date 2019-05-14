#include "type_map_tileset.hpp"
#include "../../lua_env/interface.hpp"

namespace elona
{

MapTilesetDB the_map_tileset_db;
const constexpr char* data::LuaLazyCacheTraits<_MapTilesetDBBase>::type_id;

static MapTile _convert_tile(sol::object obj, const std::string& data_id)
{
    if (obj.is<std::string>())
    {
        return MapTile{SharedId(obj.as<std::string>()), none};
    }

    if (!obj.is<sol::table>())
    {
        throw std::runtime_error("Map tile must be an ID or table");
    }

    lua::ConfigTable table(obj.as<sol::table>(), data_id);

    SharedId id(table.required<std::string>("id"));
    int chance = table.required<int>("chance");
    sol::table other_table = table.required<sol::table>("other");
    std::vector<SharedId> other;

    for (const auto kvp : other_table)
    {
        other.emplace_back(kvp.second.as<std::string>());
    }

    return MapTile{id, MapTileRandom{chance, other}};
}

static std::unordered_map<std::string, MapTile> _convert_tiles(
    sol::table table,
    const std::string& id)
{
    std::unordered_map<std::string, MapTile> tiles;

    for (const auto& kvp : table)
    {
        std::string k = kvp.first.as<std::string>();
        MapTile v = _convert_tile(kvp.second, id);
        tiles.emplace(k, v);
    }


    return tiles;
}

std::unordered_map<std::string, MapTile> MapTilesetDB::convert_tiles(
    SharedId id)
{
    auto it = this->ensure(id);
    if (it.tiles_callback)
    {
        auto args = lua::create_table(
            "stood_world_map_tile", game_data.stood_world_map_tile);
        auto result =
            it.tiles_callback->call_with_optional_result<sol::table>(args);
        if (!result)
        {
            return it.tiles;
        }
        return _convert_tiles(*result, id.get());
    }
    return it.tiles;
}

MapTileset _MapTilesetDBBase::convert(
    const lua::ConfigTable& data,
    const std::string& id)
{
    DATA_OPT_ID(parent);

    std::unordered_map<std::string, MapTile> tiles_conv;
    auto tiles = data.optional<sol::table>("tiles");
    optional<lua::WrappedFunction> tiles_callback = none;

    if (tiles)
    {
        tiles_conv = _convert_tiles(*tiles, id);
    }
    else
    {
        auto it = data.optional<sol::protected_function>("tiles");
        if (it)
        {
            tiles_callback = lua::WrappedFunction(id, *it);
        }
        else
        {
            throw std::runtime_error(
                "\"tiles\" field must be a table or function. (" + id + ")");
        }
    }

    return MapTileset{SharedId{id}, parent, tiles_conv, tiles_callback};
}

} // namespace elona
