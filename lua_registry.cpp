#include "lua.hpp"
#include "log.hpp"

namespace elona
{
namespace lua
{

namespace Registry {
void set_on_event(const std::string&, const sol::function&);
void register_chara_init(const sol::function&);
void register_map_init(const sol::function&);
}


void Registry::set_on_event(const std::string& event_id, const sol::function& callback)
{
}

// TODO merge this with Event on Lua side since the interfaces are very similar
// also, this allows registering an infinite amount of the same handler...
void Registry::register_chara_init(const sol::function& func)
{
}

// TODO merge this with Event on Lua side since the interfaces are very similar
// also, this allows registering an infinite amount of the same handler...
void Registry::register_map_init(const sol::function& func)
{
}

void init_registry(lua_env& lua)
{
    sol::table Elona = (*lua.get_state())["Elona"];
    sol::table Registry = Elona.create_named("Registry");
}

} // namespace lua
} // namespace elona
