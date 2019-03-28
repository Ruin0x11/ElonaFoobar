#include "lua_api_file.hpp"
#include "../../filesystem.hpp"

namespace elona
{
namespace lua
{

static bool _can_modify_path(const fs::path& path)
{
    if (!filesystem::file_contained_in_dir(path, filesystem::dir::user()))
    {
        return false;
    }

    return true;
}

static fs::path _validate_path(const std::string& path, bool modify = false)
{
    auto fs_path = filesystem::resolve_path_for_mod(path);
    if (!filesystem::file_contained_in_dir(fs_path, filesystem::dir::exe()))
    {
        throw sol::error(
            "Can't access the path "s + filepathutil::to_utf8_path(fs_path) +
            " as it is outside the executable directory.");
    }

    if (modify && !_can_modify_path(fs_path))
    {
        throw sol::error(
            "Can't modify "s + filepathutil::to_utf8_path(fs_path) +
            " as it is outside the user directory.");
    }

    return fs_path;
}


/**
 * @luadoc
 *
 * Creates a directory and any parent directories that do not exist.
 * @tparam string path
 */
void LuaApiFile::create_directory(const std::string& path)
{
    auto fs_path = _validate_path(path, true);

    if (!fs::create_directories(fs_path))
    {
        throw sol::error{"Failed to create directory: " +
                         filepathutil::make_preferred_path_in_utf8(fs_path)};
    }
}

/**
 * @luadoc
 *
 * Writes a string to a file, optionally appending instead of truncating.
 * @tparam string path
 * @tparam string content
 * @tparam[opt] bool append
 */
void LuaApiFile::write_file(
    const std::string& path,
    const std::string& content,
    sol::optional<bool> append)
{
    auto fs_path = _validate_path(path, true);

    if (fs::is_directory(fs_path))
    {
        throw sol::error(
            "Can't write to "s + filepathutil::to_utf8_path(fs_path) +
            " as it is a directory.");
    }

    std::ofstream out;
    if (append.value_or(false))
    {
        out.open(fs_path.native(), std::ios::binary | std::ios_base::app);
    }
    else
    {
        out.open(fs_path.native(), std::ios::binary);
    }

    out << content;
}

/**
 * @luadoc
 *
 * Copies a file or directory to path. Directories are copied recursively. If
 * <code>overwrite</code> is false or not specified, an error will be raised if
 * the source is a file and the destination already exists.
 * @tparam string source
 * @tparam string dest
 * @tparam[opt] bool overwrite
 */
void LuaApiFile::copy_path(
    const std::string& source,
    const std::string& dest,
    sol::optional<bool> overwrite)
{
    auto fs_source = _validate_path(source);
    auto fs_dest = _validate_path(dest, true);

    auto option = fs::copy_option::fail_if_exists;
    if (overwrite.value_or(false))
    {
        option = fs::copy_option::overwrite_if_exists;
    }


    fs::file_status stat(fs::symlink_status(fs_source));
    if (fs::is_directory(stat))
    {
        filesystem::copy_recursively(fs_source, fs_dest);
    }
    else if (fs::is_regular_file(stat))
    {
        fs::copy_file(fs_source, fs_dest, option);
    }
    else
    {
        throw sol::error(
            "Can't copy to "s + filepathutil::to_utf8_path(fs_dest) + ".");
    }
}

/**
 * @luadoc
 *
 * Removes a file or directory, including any child directories.
 * @tparam string path
 */
void LuaApiFile::remove_path(const std::string& path)
{
    auto fs_path = _validate_path(path, true);

    fs::remove_all(fs_path);
}

/**
 * @luadoc
 *
 * Reads the entire contents of a file to a string.
 * @tparam string path
 * @treturn string
 */
std::string LuaApiFile::read_file_to_string(const std::string& path)
{
    auto fs_path = _validate_path(path);

    if (fs::is_directory(fs_path))
    {
        throw sol::error(
            "Can't read from "s + filepathutil::to_utf8_path(fs_path) +
            " as it is a directory.");
    }

    std::ifstream in{fs_path.native(), std::ios::binary};
    return std::string(std::istreambuf_iterator<char>{in}, {});
}

/**
 * @luadoc
 *
 * Returns true if the file or directory at the given path exists.
 * @tparam string path
 * @treturn bool
 */
bool LuaApiFile::exists(const std::string& path)
{
    auto fs_path = _validate_path(path);

    return fs::exists(fs_path);
}

/**
 * @luadoc
 *
 * Returns true if the given path is a directory.
 * @tparam string path
 * @treturn bool
 */
bool LuaApiFile::is_directory(const std::string& path)
{
    auto fs_path = _validate_path(path);

    return fs::is_directory(fs_path);
}

/**
 * @luadoc
 *
 * Returns the filename component of a path to a file.
 * @tparam string path
 * @treturn string
 */
std::string LuaApiFile::filename(const std::string& path)
{
    return filepathutil::to_utf8_path(fs::path(path).filename());
}

void LuaApiFile::bind(sol::table& api_table)
{
    LUA_API_BIND_FUNCTION(api_table, LuaApiFile, create_directory);
    LUA_API_BIND_FUNCTION(api_table, LuaApiFile, write_file);
    LUA_API_BIND_FUNCTION(api_table, LuaApiFile, copy_path);
    LUA_API_BIND_FUNCTION(api_table, LuaApiFile, remove_path);
    LUA_API_BIND_FUNCTION(api_table, LuaApiFile, read_file_to_string);
    LUA_API_BIND_FUNCTION(api_table, LuaApiFile, exists);
    LUA_API_BIND_FUNCTION(api_table, LuaApiFile, is_directory);
    LUA_API_BIND_FUNCTION(api_table, LuaApiFile, filename);
}

} // namespace lua
} // namespace elona
