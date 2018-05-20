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
    std::string mod_name = (*sol.get())["Global"]["MOD_NAME"];
    ELONA_LOG("Setting " << event_id << " of " << mod_name);
    (*sol.get())["Global"]["Callbacks"][event_id] = callback;
    ELONA_LOG("Set " << event_id << " of " << mod_name);
}

void Registry::register_chara_init(const sol::function& func)
{
    sol::table inits = (*sol.get())["Elona"]["Registry"]["Inits"]["Chara"];
    inits.add(func);
}

void Registry::register_map_init(const sol::function& func)
{
    sol::table inits = (*sol.get())["Elona"]["Registry"]["Inits"]["Map"];
    inits.add(func);
}

void init_registry(std::unique_ptr<sol::state>& state)
{
    sol::table Elona = (*state.get())["Elona"];
    sol::table Registry = Elona.create_named("Registry");
    Registry.set_function("on_event", Registry::set_on_event);
    Registry.set_function("register_chara_init", Registry::register_chara_init);
    Registry.set_function("register_map_init", Registry::register_map_init);
    init_registry_storage(state);
    init_init_hooks(state);
}

void init_registry_storage(std::unique_ptr<sol::state>& state)
{
    sol::table Registry = (*state.get())["Elona"]["Registry"];
    Registry.create_named("Data");
}

void clear_registry_storage(std::unique_ptr<sol::state>& state)
{
    (*state.get())["Elona"]["Registry"]["Data"] = sol::nil;
    init_registry_storage(state);
}

void init_init_hooks(std::unique_ptr<sol::state>& state)
{
    sol::table Registry = (*state.get())["Elona"]["Registry"];
    sol::table Inits = Registry.create_named("Inits");
    Inits.create_named("Chara");
    Inits.create_named("Map");
}

void clear_init_hooks(std::unique_ptr<sol::state>& state)
{
    (*state.get())["Elona"]["Registry"]["Inits"] = sol::nil;
    init_init_hooks(state);
}

} // namespace lua
} // namespace elona
