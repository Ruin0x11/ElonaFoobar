#pragma once
#include "lua_env.hpp"
#include "../filesystem.hpp"

namespace elona
{
namespace lua
{

/***
 * Responsible for saving and loading Lua data that can be held by
 * loaded mods.
 */
class serial_manager
{
public:
    explicit serial_manager(lua_env*);

    void save_all_data(sol::environment&);
    void load_all_data(sol::environment&);
    void save_map_local_data(sol::environment&);
    void load_map_local_data(sol::environment&);

private:
    /***
     * Saves a Lua table with game-related data to a file.
     */
    void save(sol::table&, fs::path);

    /***
     * Loads game-related data into a Lua table from a file.
     */
    void load(sol::table&, fs::path);

    /***
     * The isolated Lua environment that loads the Lua serialization
     * code.
     */
    sol::environment serial_env;

    lua_env* lua;
};

} // namespace lua
} // namespace elona
