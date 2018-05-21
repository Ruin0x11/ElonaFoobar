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


void report_error(sol::error err)
{
	std::string what = err.what();
	ELONA_LOG(what);
}

// TODO mods_iterator

// TODO just make it a callback?
void on_map_loaded()
{
}

// TODO just make it a callback?
// TODO use character&, not id
void on_chara_creation(int chara_id)
{
    // TODO handle deserialization separately from creation from scratch
    // TODO only handle deserialization for characters that actually exist
    // for each mod, init its extra data for the character
    // for each mod, run chara creation callback
}

// void on_item_creation(int id)
// {
//     // for each mod, init its extra data for the item
//     // for each mod, run item creation callback
// }
//

void on_chara_removal(int id)
{
    // for each mod, invalidate global chara state
    // for each mod, run chara removal callback
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

lua_env::lua_env()
{
    lua = std::make_shared<sol::state>();
    lua->open_libraries(sol::lib::base,
                              sol::lib::package,
                              sol::lib::table,
                              sol::lib::debug,
                              sol::lib::string,
                              sol::lib::math);

    init_api(*this);
    init_registry(*this);
    init_global(*this);

    event_mgr = std::make_unique<event_manager>(this);
    event_manager::init(*this);

    //dump_state();
}

event_manager& lua_env::get_event_manager()
{
    return *event_mgr;
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

sol::environment lua_env::load_mod(const fs::path& path)
{
    // create character/item/map/global tables
    // run various mod loading stages (like defining custom fields for all prototypes in the game?)
    // evaluate init.lua to load defines
    const std::string name = path.filename().string();
    create_named_registry(name);

    sol::environment env(*(this->lua), sol::create, this->lua->globals());
    env["Global"]["MOD_NAME"] = name;

    auto result = this->lua->safe_script_file(filesystem::make_preferred_path_in_utf8(path / "init.lua"), env);
    if (!result.valid())
    {
        sol::error err = result;
        report_error(err);
        throw new std::runtime_error("Failed initializing mod "s + name);
    }
    // determine mod overrides inside .json files
    // merge overrides, new things, and locale configs into global database
    // add reference to global API table as Elona so the mod can use it

    return env;
}

void lua_env::load_all_mods(const fs::path& mods_dir)
{
    const std::string init_script = "init.lua";

    // TODO: [dependency management] order mods and always load core first.
    for (const auto& entry : filesystem::dir_entries{
             mods_dir, filesystem::dir_entries::type::dir})
    {
        if(fs::exists(entry.path() / init_script))
        {
            const std::string mod_name = entry.path().filename().string();
            ELONA_LOG("Found mod " << mod_name);

            mod_info info;
            info.name = mod_name;
            this->mods.push_back(std::move(info));
        }
    }

    for (auto &&mod : this->mods)
    {
        mod.env = load_mod(mods_dir / mod.name);
        lua::store mod_store;
        mod_store.init(*this->get_state(), mod.env);
        mod.store = mod_store;
        ELONA_LOG("Loaded mod " << mod.name);
    }
}

void run_file(const fs::path& path)
{

}

void run_startup_script(const std::string& name)
{

}

} // namespace lua
} // namespace elona
