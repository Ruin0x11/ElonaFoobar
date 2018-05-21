#include "lua.hpp"
#include "elona.hpp"
#include "config.hpp"
#include "event_manager.hpp"
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

lua_env lua;

// TODO wrap all table editing methods in their own functions
// TODO make everything less messy
// TODO move api to separate source file
// TODO make things immutable where they need to be

lua_env::lua_env()
{
    lua = std::make_shared<sol::state>();
    lua->open_libraries(sol::lib::base,
                              sol::lib::package,
                              sol::lib::table,
                              sol::lib::debug,
                              sol::lib::string,
                              sol::lib::math);

    // Create dummy Store table to prevent crashes on access from state outside of an environment
    lua->create_named_table("Store");

    init_api(*this);
    init_registry(*this);
    init_global(*this);

    event_mgr = std::make_unique<event_manager>(this);
    event_manager::init(*this);

    //dump_state();

    scan_all_mods(filesystem::dir::mods());
    load_core_mod(filesystem::dir::mods());
}

event_manager& lua_env::get_event_manager()
{
    return *event_mgr;
}

void report_error(sol::error err)
{
	std::string what = err.what();
	ELONA_LOG(what);
}

// TODO mods_iterator

void lua_env::on_chara_creation(character& chara)
{
    // TODO handle deserialization separately from creation from scratch
    // TODO only handle deserialization for characters that actually exist
    // for each mod, init its extra data for the character
    // for each mod, run chara creation callback
    for(const auto& callback : this->get_event_manager().get_callbacks(event_kind_t::character_initialized))
    {
        sol::table retval = callback.function.call();
        sol::state_view view(*lua);
        retval.for_each([&](sol::object key, sol::object value)
        {
            if(!key.is<std::string>())
            {
                assert(0);
            }
            else
            {
                mods.at(callback.mod_name).store.set(key.as<std::string>(), value, view);
            }
        });
    }
}

 void on_item_creation(int id)
 {
     // for each mod, init its extra data for the item
     // for each mod, run item creation callback
     //this->get_event_manager()->run_callbacks<event_kind_t::item_initialized>(item);
 }


void lua_env::on_chara_removal(character&)
{
    // for each mod, invalidate global chara state
    // for each mod, run chara removal callback
    //this->get_event_manager()->run_callbacks<event_kind_t::character_removed>(chara);
}
//
// void on_item_removal(int id)
// {
//     // for each mod, invalidate global item state
//     // for each mod, run item removal callback
// }


void init_global(lua_env& lua)
{
    sol::table Global = lua.get_state()->create_named_table("Global");
    Global.create_named("Callbacks");
    Global.create_named("Init");
}

/***
 * This does various things:
 * - Creates a new sol::environment for the script instance to run.
 * - Registers the new environment in the known list of environments.
 * - Makes the core API tables read-only.
 *
 * This shouldn't be called if the mod won't be executing any lua code
 * (only has data.lua)
 */
void create_new_environment(const std::string& id)
{

}

void create_named_registry(const std::string& name)
{

}

void lua_env::load_mod_in_global_env(const fs::path& path, mod_info& mod)
{
    auto result = this->lua->safe_script_file(filesystem::make_preferred_path_in_utf8(path / mod.name / "init.lua"));
    if (!result.valid())
    {
        sol::error err = result;
        report_error(err);
        throw new std::runtime_error("Failed initializing mod "s + mod.name);
    }
}

void lua_env::load_mod(const fs::path& path, mod_info& mod)
{
    // create character/item/map/global tables
    // run various mod loading stages (like defining custom fields for all prototypes in the game?)
    // evaluate init.lua to load defines
    auto result = this->lua->safe_script_file(filesystem::make_preferred_path_in_utf8(path / mod.name / "init.lua"), mod.env);
    if (!result.valid())
    {
        sol::error err = result;
        report_error(err);
        throw new std::runtime_error("Failed initializing mod "s + mod.name);
    }
    // determine mod overrides inside .json files
    // merge overrides, new things, and locale configs into global database
    // add reference to global API table as Elona so the mod can use it
}

void lua_env::scan_all_mods(const fs::path& mods_dir)
{
    if(stage != mod_stage_t::not_started)
    {
        throw new std::runtime_error("Mods have already been scanned!");
    }

    const std::string init_script = "init.lua";

    // TODO: [dependency management] order mods and always load core first.
    for (const auto& entry : filesystem::dir_entries{
             mods_dir, filesystem::dir_entries::type::dir})
    {
        if(fs::exists(entry.path() / init_script))
        {
            const std::string mod_name = entry.path().filename().string();
            ELONA_LOG("Found mod " << mod_name);

            if(mod_name == "script")
            {
                throw new std::runtime_error("\"script\" is a reserved mod name.");
            }

            mod_info info;
            create_mod_info(mod_name, info);
            this->mods.emplace(mod_name, std::move(info));
        }
    }
    stage = mod_stage_t::scan_finished;
}

void lua_env::load_core_mod(const fs::path& mods_dir)
{
    if(stage != mod_stage_t::scan_finished)
    {
        throw new std::runtime_error("Mods haven't been scanned yet!");
    }

    auto val = this->mods.find("core");
    if (val == this->mods.end())
    {
        throw new std::runtime_error("Core mod was not found. Does \"mods/core\" exist?");
    }

    // Load the core mod before any others, because there is a need
    // for things on the Lua side to be made read-only after the core
    // mod is loaded.
    load_mod_in_global_env(mods_dir, val->second);

    // Make the core API table read-only.
    lua->safe_script(R"(Elona = Elona.ReadOnly.make_read_only(Elona))");

    stage = mod_stage_t::core_mod_loaded;
}

void lua_env::load_all_mods(const fs::path& mods_dir)
{
    if(stage != mod_stage_t::core_mod_loaded)
    {
        throw new std::runtime_error("Core mod wasn't loaded!");
    }
    for (auto &&pair : this->mods)
    {
        auto mod = pair.second;
        if(mod.name == "core")
        {
            continue;
        }
        else
        {
            load_mod(mods_dir, mod);
        }
        ELONA_LOG("Loaded mod " << mod.name);
    }

    stage = mod_stage_t::all_mods_loaded;
}

void lua_env::create_mod_info(const std::string& name, mod_info& mod)
{
    mod.name = name;
    mod.env = sol::environment(*(this->lua), sol::create, this->lua->globals());
    mod.env["Global"]["MOD_NAME"] = name;
    mod.store = lua::store();
    mod.store.init(*this->get_state(), mod.env);
    mod.env["Store"] = mod.store;
}

void lua_env::run_startup_script(const std::string& name)
{
    if(stage < mod_stage_t::core_mod_loaded)
    {
        throw new std::runtime_error("Core mod wasn't loaded!");
    }
    if(this->mods.find(name) != this->mods.end())
    {
        throw new std::runtime_error("Startup script was already run.");
    }

    mod_info script_mod;
    create_mod_info("script", script_mod);

    lua->safe_script_file(filesystem::make_preferred_path_in_utf8(
                              filesystem::dir::data() / "script"s / name),
        script_mod.env);

    // The startup script is special since everything is deferred until the map loads.
    // So, re-run the map/character initialization things to pick up new init hooks loaded by the startup script.
    // The following shouldn't cause any significant changes in behavior.
    this->get_event_manager().run_callbacks<event_kind_t::map_initialized>();
    for (int chara_id = 0; chara_id < ELONA_MAX_CHARACTERS; chara_id++) {
        if(elona::cdata[chara_id].state != 0)
        {
            on_chara_creation(elona::cdata[chara_id]);
        }
    }
    ELONA_LOG("Loaded startup script " << name);

    this->mods.emplace("script", std::move(script_mod));
}


// For testing use
void lua_env::load_mod_from_script(const std::string& name, const std::string& script)
{
    if(stage < mod_stage_t::core_mod_loaded)
    {
        throw new std::runtime_error("Core mod wasn't loaded!");
    }
    {
        auto val = mods.find(name);
        if(val != mods.end())
            throw new std::runtime_error("Mod "s + name + " was already initialized."s);
    }

    mod_info info;
    create_mod_info(name, info);

    auto result = this->lua->safe_script(script, info.env);
    if (!result.valid())
    {
        sol::error err = result;
        report_error(err);
        throw new std::runtime_error("Failed initializing mod "s + info.name);
    }

    this->mods.emplace(name, std::move(info));
}

void lua_env::run_in_mod(const std::string& name, const std::string& script)
{
    auto val = mods.find(name);
    if(val == mods.end())
        throw new std::runtime_error("No such mod "s + name + "."s);
    this->lua->script(script, val->second.env);
}

} // namespace lua
} // namespace elona
