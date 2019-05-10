#pragma once
#include "../../lua_env/wrapped_function.hpp"
#include "../lua_lazy_cache.hpp"



namespace elona
{

struct MapObject
{
    int legacy_id;
    lua::WrappedFunction on_activate;
};



ELONA_DEFINE_LUA_DB(MapObjectDB, MapObject, true, "core.map_object")



extern MapObjectDB the_map_object_db;

} // namespace elona
