#pragma once

#include "thirdparty/sol2/sol.hpp"
#include "character.hpp"
#include "event_manager.hpp"
#include "filesystem.hpp"
#include "handle_manager.hpp"
#include "item.hpp"
#include "lua_store.hpp"
#include <map>
#include <vector>

namespace elona
{
namespace lua
{

using namespace std::literals::string_literals;

class event_manager;
class handle_manager;

struct mod_info
{
    std::string name;
    sol::environment env;
    lua::store store;
};

enum class mod_stage_t : unsigned
{
    not_started,
    scan_finished,
    core_mod_loaded,
    all_mods_loaded,
};

class lua_env
{
public:
    lua_env();
    ~lua_env() = default;
    std::shared_ptr<sol::state> get_state() { return lua; }

    void scan_all_mods(const fs::path&);
    void load_core_mod(const fs::path&);
    void load_all_mods(const fs::path&);

    void run_startup_script(const std::string&);

    void on_chara_creation(character&);
    void on_item_creation(item&);
    void on_chara_removal(character&);
    void on_item_removal(item&);

    event_manager& get_event_manager();
    handle_manager& get_handle_manager();

    // For testing use
    void load_mod_from_script(const std::string& name, const std::string& script);
    void run_in_mod(const std::string& name, const std::string& script);
    const mod_info& get_mod(const std::string& name) const {
        auto val = mods.find(name);
        if(val == mods.end())
            throw new std::runtime_error("No such mod "s + name + "."s);
        return val->second;
    }
private:
    void load_mod(const fs::path& path, mod_info&);
    void load_mod_in_global_env(const fs::path& path, mod_info& mod);
    void create_mod_info(const std::string&, mod_info&);
private:
    std::shared_ptr<sol::state> lua;
    std::unique_ptr<event_manager> event_mgr;
    std::unique_ptr<handle_manager> handle_mgr;
    std::unordered_map<std::string, mod_info> mods;
    mod_stage_t stage = mod_stage_t::not_started;
};

void init();
void init_api(lua_env&);
void init_registry(lua_env&);
void init_global(lua_env&);
void init_registry_storage(lua_env&);
void clear();
void clear_registry_storage(lua_env&);
void init_init_hooks(lua_env&);
void clear_init_hooks(lua_env&);
void create_named_registry(const std::string&);
void run_file(const fs::path&);
void on_map_loaded();

extern lua_env lua;

}
}
