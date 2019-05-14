#pragma once
#include <unordered_map>
#include "../../filesystem.hpp"
#include "../../lua_env/wrapped_function.hpp"
#include "../lua_lazy_cache.hpp"

namespace elona
{

struct MapTileRandom
{
    int chance;
    std::vector<SharedId> other;
};

struct MapTile
{
    SharedId id;
    optional<MapTileRandom> random;
};

struct MapTileset
{
    SharedId id;
    optional<SharedId> parent;
    std::unordered_map<std::string, MapTile> tiles;
    optional<lua::WrappedFunction> tiles_callback;
};


ELONA_DEFINE_LUA_DB(_MapTilesetDBBase, MapTileset, false, "core.map_tileset")



class MapTilesetDB : public _MapTilesetDBBase
{
public:
    std::unordered_map<std::string, MapTile> convert_tiles(SharedId id);

private:
};

extern MapTilesetDB the_map_tileset_db;

} // namespace elona
