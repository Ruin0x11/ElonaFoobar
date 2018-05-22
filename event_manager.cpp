#include "event_manager.hpp"

#include "log.hpp"
#include "lua.hpp"

namespace elona
{
namespace lua
{

void event_manager::init(lua_env& lua)
{
    sol::table Elona = (*lua.get_state())["Elona"];
    sol::table Event = Elona.create_named("Event");

    Event.set_function("register", [&lua](event_kind_t event,
                                          sol::function func,
                                          sol::this_environment this_env) {
                           sol::environment& env = this_env;
                           lua.get_event_manager().register_event(event, env, func);
                       });

    Event.set_function("trigger", [&lua](event_kind_t event,
                                         sol::table data) {
                           lua.get_event_manager().trigger_event(event, data);
                       });
}

event_manager::event_manager(lua_env* lua)
{
    this->lua = lua;

    sol::table Defines = (*lua->get_state())["Elona"]["Defines"];

    Defines["EventKind"] = Defines.create_with(
        "CharaMoved", event_kind_t::chara_moved,
        "PlayerTurn", event_kind_t::player_turn,
        "AllTurnsFinished", event_kind_t::all_turns_finished,
        "MapInitialized", event_kind_t::map_initialized,
        "CharaCreated", event_kind_t::character_created,
        "CharaRemoved", event_kind_t::character_removed,
        "ItemCreated", event_kind_t::item_created,
        "ItemRemoved", event_kind_t::item_removed
        );
    init_events();
}

void event_manager::init_events()
{
    unsigned event_count = static_cast<unsigned>(event_kind_t::COUNT);
    for(unsigned i = 0; i < event_count; i++)
    {
        event_kind_t event_kind = static_cast<event_kind_t>(i);
        events[event_kind] = callbacks();
    }
}

void event_manager::register_event(event_kind_t event,
                                   sol::environment& env,
                                   sol::function& callback)
{
    auto iter = events.find(event);
    if(iter != events.end())
    {
        iter->second.push(env, callback);
    }
}

void event_manager::trigger_event(event_kind_t event,
                                  sol::table data)
{
    auto iter = events.find(event);
    if(iter != events.end())
    {
        iter->second.run(callbacks::retval_type<void>(), data);
    }
    else
    {
        ELONA_LOG("No callbacks registered for event " << static_cast<int>(event) << ", skipping.");
    }
}

void event_manager::clear()
{
    events.clear();
    init_events();
}

}
}
