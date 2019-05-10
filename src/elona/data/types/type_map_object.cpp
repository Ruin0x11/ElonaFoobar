#include "type_map_object.hpp"



namespace elona
{

MapObjectDB the_map_object_db;
const constexpr char* data::LuaLazyCacheTraits<MapObjectDB>::type_id;



MapObject MapObjectDB::convert(
    const lua::ConfigTable& data,
    const std::string& id)
{
    auto legacy_id = data.required<int>("legacy_id");
    DATA_REQ_FUNC(on_activate);

    return MapObject{legacy_id, on_activate};
}

} // namespace elona
