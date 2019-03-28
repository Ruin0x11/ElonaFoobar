#include "filesystem.hpp"
#include "defines.hpp"



namespace
{

fs::path get_executable_dir()
{
    static auto cache = ([] {
        auto exe_name = filepathutil::get_executable_path();
        if (!exe_name)
        {
            throw std::runtime_error(u8"Error: fail to get excutable path");
        }
        return fs::canonical(fs::path{*exe_name}.remove_filename());
    })();

    return cache;
}

} // namespace



namespace elona
{
namespace filesystem
{

namespace dir
{

fs::path current_profile_dir;
fs::path base_mod_dir;
fs::path base_save_dir;
fs::path base_user_dir;


#define ELONA_DEFINE_PREDEFINED_DIR(func_name, path_name) \
    fs::path func_name() \
    { \
        return filesystem::path(path_name); \
    }

ELONA_DEFINE_PREDEFINED_DIR(exe, "")
ELONA_DEFINE_PREDEFINED_DIR(data, "data")
ELONA_DEFINE_PREDEFINED_DIR(graphic, "graphic")
ELONA_DEFINE_PREDEFINED_DIR(locale, "locale")
ELONA_DEFINE_PREDEFINED_DIR(log, "log")
ELONA_DEFINE_PREDEFINED_DIR(map, "map")
ELONA_DEFINE_PREDEFINED_DIR(sound, "sound")
ELONA_DEFINE_PREDEFINED_DIR(tmp, "tmp")

#undef ELONA_DEFINE_PREDEFINED_DIR



fs::path profile_root()
{
    return filesystem::path("profile");
}



fs::path current_profile()
{
    return current_profile_dir;
}



fs::path mod()
{
    return base_mod_dir;
}



fs::path save()
{
    return base_save_dir;
}



fs::path user()
{
    return base_user_dir;
}



void set_current_profile_directory(const fs::path& current_profile_dir)
{
    dir::current_profile_dir = current_profile_dir;
}



void set_base_mod_directory(const fs::path& base_mod_dir)
{
    dir::base_mod_dir = base_mod_dir;
}



void set_base_save_directory(const fs::path& base_save_dir)
{
    dir::base_save_dir = base_save_dir;
}



void set_base_user_directory(const fs::path& base_user_dir)
{
    dir::base_user_dir = base_user_dir;
}



fs::path for_mod(const std::string& mod_id)
{
    return mod() / filepathutil::u8path(mod_id);
}



fs::path save(const std::string& player_id)
{
    return save() / filepathutil::u8path(player_id);
}



fs::path user_script()
{
    return user() / u8"script";
}



void set_profile_directory(const fs::path& profile_dir)
{
    set_current_profile_directory(profile_dir);
    set_base_mod_directory(profile_dir / u8"mod");
    set_base_save_directory(profile_dir / u8"save");
    set_base_user_directory(profile_dir / u8"user");
}

} // namespace dir



fs::path path(const std::string& str)
{
    return get_executable_dir() / filepathutil::u8path(str);
}



fs::path resolve_path_for_mod(const std::string& mod_local_path)
{
    // TODO: standardize mod naming convention.
    std::regex mod_name_regex("^__([a-zA-Z0-9_]+)__/(.*)");
    std::smatch match;
    std::string mod_name;
    fs::path rest;

    if (std::regex_match(mod_local_path, match, mod_name_regex) &&
        match.size() == 3)
    {
        mod_name = match.str(1);
        rest = filepathutil::to_utf8_path(match.str(2));
    }
    else
    {
        throw std::runtime_error("Invalid filepath syntax: " + mod_local_path);
    }

    if (mod_name == "BUILTIN")
    {
        return dir::exe() / rest;
    }
    else if (mod_name == "USER")
    {
        return dir::user() / rest;
    }
    else
    {
        return dir::for_mod(mod_name) / rest;
    }
}



bool file_contained_in_dir(fs::path file, const fs::path& base_path)
{
    // Modifies filename, so copy is needed.
    if (!file.has_filename())
    {
        return false;
    }
    file.remove_filename();

    // Strip "." and ".."
    file = fs::weakly_canonical(file);
    fs::path dir = fs::weakly_canonical(base_path);

    std::size_t dir_len = std::distance(dir.begin(), dir.end());
    std::size_t file_len = std::distance(file.begin(), file.end());

    if (dir_len > file_len)
    {
        return false;
    }

    bool dir_is_prefix = std::equal(dir.begin(), dir.end(), file.begin());
    return dir_is_prefix;
}



void copy_recursively(const fs::path& source, const fs::path& destination)
{
    // Check pre-conditions.
    if (!(fs::exists(source) && fs::is_directory(source)))
    {
        throw std::runtime_error{
            "Source does not exist, or isn't a directory: " +
            filepathutil::make_preferred_path_in_utf8(source)};
    }
    if (fs::exists(destination))
    {
        throw std::runtime_error{
            "Destination already exists: " +
            filepathutil::make_preferred_path_in_utf8(destination)};
    }

    // mkdir destination
    if (!fs::create_directories(destination))
    {
        throw std::runtime_error{
            "Failed to create directory: " +
            filepathutil::make_preferred_path_in_utf8(destination)};
    }

    // Iterate all files under source.
    for (const auto& entry : fs::directory_iterator{source})
    {
        const auto from = entry.path();
        const auto to = destination / from.filename();
        if (fs::is_directory(from))
        {
            // Call itself recursively.
            copy_recursively(from, to);
        }
        else
        {
            fs::copy_file(from, to);
        }
    }
}

} // namespace filesystem
} // namespace elona
