#pragma once
#include "thirdparty/sol2/sol.hpp"
#include "filesystem.hpp"
#include <map>

namespace elona
{
namespace lua
{

extern std::unique_ptr<sol::state> sol;

void init();
void init_api(std::unique_ptr<sol::state>&);
void init_registry(std::unique_ptr<sol::state>&);
void init_global(std::unique_ptr<sol::state>&);
void init_registry_storage(std::unique_ptr<sol::state>&);
void clear();
void clear_registry_storage(std::unique_ptr<sol::state>&);
void init_init_hooks(std::unique_ptr<sol::state>&);
void clear_init_hooks(std::unique_ptr<sol::state>&);
void create_named_registry(const std::string&);
void run_file(const fs::path&);
void run_startup_script(const std::string&);
void callback(const std::string&);
void callback(const std::string&, const std::map<std::string, int>);
void load_mod(const std::string&);
void on_map_loaded();
void on_chara_creation(int id);
void on_chara_removal(int id);
void reload();
void dump_state();

}
}
