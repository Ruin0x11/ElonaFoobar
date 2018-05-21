#pragma once

#include "thirdparty/sol2/sol.hpp"
#include "filesystem.hpp"
#include "event_manager.hpp"
#include "lua_store.hpp"
#include <map>
#include <vector>

namespace elona
{
namespace lua
{

class event_manager;

struct mod_info
{
    std::string name;
    sol::environment env;
    lua::store store;
};

class lua_env
{
public:
    lua_env();
    ~lua_env() = default;
    std::shared_ptr<sol::state> get_state() { return lua; }
    void load_all_mods(const fs::path&);
    void run_startup_script(const std::string&);

    event_manager& get_event_manager();

private:
    sol::environment load_mod(const fs::path&);
private:
    std::shared_ptr<sol::state> lua;
    std::unique_ptr<event_manager> event_mgr;
    std::vector<mod_info> mods;
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
void on_chara_creation(int id);
void on_chara_removal(int id);

extern lua_env lua;

}
}
