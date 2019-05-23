#pragma once
#include <set>
#include <string>
#include <vector>
#include "../filesystem.hpp"
#include "data_table.hpp"
#include "lua_env.hpp"

namespace elona
{
namespace lua
{

struct ModInfo;

struct ScriptLoadOrder
{
    using ScriptsInPriority =
        std::unordered_map<std::string, std::vector<fs::path>>;

    void clear()
    {
        priorities.clear();
        storage.clear();
    }

    void add_priority(int priority)
    {
        priorities.insert(priority);
        storage[priority] = ScriptsInPriority{};
    }

    void add_script(int priority, const std::string& mod_id, fs::path script)
    {
        if (priorities.find(priority) == priorities.end())
        {
            add_priority(priority);
        }

        auto& scripts = storage.at(priority);

        if (scripts.find(mod_id) == scripts.end())
        {
            scripts[mod_id] = {};
        }

        storage.at(priority).at(mod_id).emplace_back(script);
    }

    int next_priority_for_mod(const std::string& mod_id)
    {
        int priority = 100;

        for (const auto& pair : storage)
        {
            const auto& for_priority = pair.second;
            if (for_priority.find(mod_id) != for_priority.end())
            {
                priority = std::max(priority, pair.first + 1);
            }
        }

        return priority;
    }

    std::set<int> priorities;
    std::unordered_map<int, ScriptsInPriority> storage;
};

/***
 * Stores arbitrary data as Lua tables in a naive object database
 * manner. Intended for immutable definitions of things like
 * characters and items. Not as efficient performance-wise when
 * accessed from C++ as compared to C++-level data structures (due to
 * the overhead of crossing the Lua boundary and type checking).
 * Rebuilt on startup and kept in memory. If memory usage becomes an
 * issue then data could be serialized in a Lua-readable format, or
 * stored in an actual database like SQLite.
 */
class DataManager
{
public:
    explicit DataManager(LuaEnv* lua);

    void clear();

    void init_from_mods();

    int next_script_priority_for_mod(const std::string& mod_id)
    {
        return _scripts.next_priority_for_mod(mod_id);
    }

    void add_data_script(
        const std::string& filename,
        int priority,
        sol::environment env);

    DataTable& get()
    {
        return _data;
    }

private:
    void _init_from_mod(ModInfo& mod, const fs::path& script);

    DataTable _data;
    LuaEnv* _lua;

    ScriptLoadOrder _scripts;
};

} // namespace lua
} // namespace elona
