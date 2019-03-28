#pragma once
#include <unordered_map>
#include "../../thirdparty/sol2/sol.hpp"
#include "../filesystem.hpp"

namespace elona
{
namespace lua
{


/**
 * Acts as Lua's package.loaded and require, but local to a single Lua
 * environment.
 *
 * This is needed since package.loaded is normally global to the entire Lua
 * state.
 */
class LoadedChunkCache
{
public:
    LoadedChunkCache(const fs::path& base_path_)
        : base_path(base_path_)
    {
        assert(fs::exists(base_path));
    }

public:
    sol::object
    require(const std::string& name, sol::environment env, sol::state& state)
    {
        auto it = chunk_cache.find(name);
        if (it != chunk_cache.end())
            return it->second;

        const fs::path full_path = base_path / (name + ".lua");
        if (!(fs::exists(full_path) &&
              filesystem::file_contained_in_dir(full_path, base_path)))
            return sol::lua_nil;

        sol::object result =
            state.script_file(filepathutil::to_utf8_path(full_path), env);

        if (result != sol::lua_nil)
            chunk_cache[name] = result;

        return result;
    }

private:
    fs::path base_path;
    std::unordered_map<std::string, sol::object> chunk_cache;
};

} // namespace lua
} // namespace elona
