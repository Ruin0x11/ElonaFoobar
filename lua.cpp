#include "lua.hpp"
#include "elona.hpp"
#include "character.hpp"
#include "filesystem.hpp"
#include "log.hpp"
#include "variables.hpp"
#include <vector>
#include <map>

namespace elona
{
namespace lua
{

// TODO wrap all table editing methods in their own functions
// TODO make everything less messy
// TODO move api to separate source file
// TODO make things immutable where they need to be

std::unique_ptr<sol::state> sol;

void reload()
{
    // TODO more sophisticated reloading
    sol.get()->script("if Event then Event.clear_all() end");

    load_mod("core");

    txt("Reloaded core/init.lua. ");
}

void run_file(const fs::path& filepath)
{
    sol.get()->script_file(filepath.string());
}

void load_mod(const std::string& name)
{
    (*sol.get())["Global"]["MOD_NAME"] = name;
    // create character/item/map/global tables

    // TODO this could overwrite a thing if the mod is already loaded
    sol::table data = (*sol.get())["Elona"]["Registry"]["Data"];

    sol::table modlocal = data.create_named(name);

    sol::table Chara = modlocal.create_named("Chara");
    sol::table Item = modlocal.create_named("Item");
    sol::table Map = modlocal.create_named("Map");
    sol::table Global = modlocal.create_named("Global");
    // run various mod loading stages (like defining custom fields for all prototypes in the game?)
    // evaluate init.lua to load defines
    sol.get()->script_file("mods/"s + name + "/init.lua"s);
    // determine mod overrides inside .json files
    // merge overrides, new things, and locale configs into global database
    // add reference to global API table as Elona so the mod can use it
    (*sol.get())["Global"]["MOD_NAME"] = "";
}

void callback(const std::string& event_id)
{
    callback(event_id, {});
}

void callback(const std::string& event_id, const std::map<std::string, int> args)
{
    // for each mod, if there is an array of callbacks at the given name in its exports, run all of them in order
    // could use hashset to determine quickly which mods define callbacks
    // or, use a plain list with references to the exported callbacks of each mod
    ELONA_LOG("Running callback " << event_id);
    sol::optional<sol::protected_function> func = (*sol.get())["Global"]["Callbacks"][event_id];
    if (func && func.value() != sol::nil)
    {
        auto result = func.value()(event_id, args);
        if (!result.valid())
        {
            txtef(3);
            sol::error err = result;
            std::string what = err.what();
            ELONA_LOG(what);
            txt(what);
        }
    }
}
//
// sol::any call(const std::string& mod_name, const std::string& func_name)
// {
//     // validate the mod exists
//     // try to see if the mod's exports define the named function
//     // if so, call it and marshal its result back to the caller
//     // if not, return nil
// }
//
// int formula(const std::string& id)
// {
//     // use a hashmap to look up which mod has the formula, then call it and return the result
//     // TODO think about performance overhead of having multiple mods run formulas in turn
//     return 0;
// }
//
// void on_save()
// {
//     // for each mod, serialize everything
       // TODO if character/item reference, serialize as integer id
// }
//
// void on_load()
// {
//     // for each mod, deserialize everything
// }
//
// void on_map_initialize()
// {
//     // for each mod, deserialize its map/chara/item tables from the new map, or initialize if nonexisting
//     // run mod callbacks for map initialization
// }
//
// void on_map_exit()
// {
//     // for each mod, serialize its map/chara/item tables
//     // run mod callbacks for map exit
// }
//

void report_error(sol::error err)
{
    std::string what = err.what();
    ELONA_LOG(what);
}

void initialize_mod_data_for_chara(int chara, const std::string& mod_name, sol::table& data)
{
    sol::table inits = (*sol.get())["Registry"]["Inits"]["Chara"];

    for(const auto& pair : inits)
    {
        auto func = pair.second.as<sol::protected_function>();
        if(func && func != sol::nil) {
            auto initial_mod_data = func(chara); // TODO except player/allies/respawnable characters
            if (initial_mod_data.valid())
            {
                data[mod_name]["Chara"][chara] = initial_mod_data;
            }
            else
            {
                sol::error error = initial_mod_data;
                report_error(error);
            }
        }
    }
}


void initialize_mod_data_for_map(const std::string& mod_name, sol::table& data)
{
    sol::table inits = (*sol.get())["Registry"]["Inits"]["Map"];

    for(const auto& pair : inits)
    {
        auto func = pair.second.as<sol::protected_function>();
        if(func && func != sol::nil) {
            int width = mdata(0);
            int height = mdata(1);
            auto initial_mod_data = func(width, height);
            if (initial_mod_data.valid())
            {
                data[mod_name]["Map"] = initial_mod_data;
            }
            else
            {
                sol::error error = initial_mod_data;
                report_error(error);
            }
        }
    }
}

// TODO mods_iterator

// TODO just make it a callback?
void on_map_creation()
{
    sol::table registry_data = (*sol.get())["Elona"]["Registry"]["Data"];
    for(const auto& pair : registry_data)
    {
        const std::string mod_name = pair.first.as<std::string>();
        initialize_mod_data_for_map(mod_name, registry_data);
        //callback("map_created");
    }
}

// TODO just make it a callback?
// TODO use character&, not id
void on_chara_creation(int chara_id)
{
    // TODO handle deserialization separately from creation from scratch
    // TODO only handle deserialization for characters that actually exist
    // for each mod, init its extra data for the character
    // for each mod, run chara creation callback
    if(elona::cdata[chara_id].state == 0)
    {
        return;
    }

    sol::table registry_data = (*sol.get())["Elona"]["Registry"]["Data"];
    for(const auto& pair : registry_data)
    {
        const std::string mod_name = pair.first.as<std::string>();
        initialize_mod_data_for_chara(chara_id, mod_name, registry_data);
        //callback("chara_created", {{"chara", chara_id}});
    }
}

// void on_item_creation(int id)
// {
//     // for each mod, init its extra data for the item
//     // for each mod, run item creation callback
// }
//

void on_chara_removal(int id)
{
    ELONA_LOG("Character removed. Here is the data that was lost.\n")
    sol::table data = (*sol.get())["Elona"]["Registry"]["Data"];
    for(auto& pair : data)
    {
        const std::string mod_name = pair.first.as<std::string>();
        data[mod_name]["Chara"][id] = sol::nullopt; // TODO except player/allies/respawnable characters
    }

    // for each mod, invalidate global chara state
    // for each mod, run chara removal callback
}
//
// void on_item_removal(int id)
// {
//     // for each mod, invalidate global item state
//     // for each mod, run item removal callback
// }


void init_global(std::unique_ptr<sol::state>& state)
{
    sol::table Global = state.get()->create_named_table("Global");
    Global.create_named("Callbacks");
    Global.create_named("Init");
}

void dump_state()
{
    sol.get()->script("if Elona.Debug and Elona.Debug.inspect then Elona.Debug.inspect(Elona) end");
}

void init()
{
    sol = std::make_unique<sol::state>();
    sol.get()->open_libraries(sol::lib::base,
                              sol::lib::package,
                              sol::lib::table,
                              sol::lib::debug,
                              sol::lib::string);

    init_api(sol);
    init_registry(sol);
    init_global(sol);

    // prevent usage of some tables during mod loading, since calling things like GUI.txt at the top level before starting the game is dangerous
    // load core mod first
    load_mod("core");
    // for each other mod, load_mod()
    // after global prototype db is made, make it read-only
    // make stdlib table Elona read-only
    dump_state();
}

} // namespace lua
} // namespace elona
