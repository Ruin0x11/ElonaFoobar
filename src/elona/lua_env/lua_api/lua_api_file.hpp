#pragma once
#include "lua_api_common.hpp"

namespace elona
{

namespace lua
{

/**
 * @luadoc
 *
 * Functions for reading/writing mod-related files. All paths are assumed to use
 * the mod path syntax, like "__core__/graphic/file.bmp".
 *
 * There are two special mod names you can use:
 *   - <code>__BUILTIN__</code>: the root executable directory.
 *   - <code>__USER__</code>: the user/ directory.
 *
 * You can read any path inside the game's directory, but paths can only be
 * written to/deleted from the user/ folder.
 */
namespace LuaApiFile
{
void create_directory(const std::string&);

void write_file(const std::string&, const std::string&, sol::optional<bool>);

void copy_path(const std::string&, const std::string&, sol::optional<bool>);

void remove_path(const std::string&);

std::string read_file_to_string(const std::string&);

bool exists(const std::string&);

bool is_directory(const std::string&);

std::string filename(const std::string&);

void bind(sol::table&);
} // namespace LuaApiFile

} // namespace lua
} // namespace elona
