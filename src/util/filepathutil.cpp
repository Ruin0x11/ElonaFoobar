#include "filepathutil.hpp"
#include <algorithm>
#include <boost/predef.h>

// For get_executable_path()
#if BOOST_OS_WINDOWS
#include <codecvt> // std::codecvt_utf8_utf16
#include <Shlobj.h> // SHGetKnownFolderPath
#include <windows.h> // GetModuleFileName
#elif BOOST_OS_MACOS
#include <limits.h> // PATH_MAX
#include <mach-o/dyld.h> // _NSGetExecutablePath
#include <pwd.h> // getpwuid
#include <stdlib.h> // getenv
#include <sys/types.h> // struct passwd
#elif BOOST_OS_LINUX
#include <limits.h> // PATH_MAX
#include <pwd.h> // getpwuid
#include <sys/types.h> // struct passwd
#include <unistd.h> // readlink
#elif BOOST_OS_ANDROID
#include "SDL_system.h" // SDL_AndroidGetExternalStoragePath
#else
#error Unsupported OS
#endif


namespace filepathutil
{

namespace fs = boost::filesystem;



/**
 * Converts a UTF-8 encoded string to a filesystem path in the platform's native
 * encoding (UTF-16 for Windows, UTF-8 for everything else)
 */
fs::path u8path(const std::string& str)
{
#if BOOST_OS_WINDOWS
    return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>()
        .from_bytes(str);
#else
    return str;
#endif
}



/**
 * Returns a UTF-8 string from a filesystem path with all directory separators
 * converted to the platform's preferred separators ('\' for Windows, '/' for
 * everything else)
 */
std::string make_preferred_path_in_utf8(const fs::path& path)
{
#if BOOST_OS_WINDOWS
    return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>()
        .to_bytes(path.make_preferred().native());
#else
    return path.native();
#endif
}



/**
 * Returns a UTF-8 string from a filesystem path.
 */
std::string to_utf8_path(const fs::path& path)
{
#if BOOST_OS_WINDOWS
    return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>()
        .to_bytes(path.native());
#else
    return path.native();
#endif
}



std::string to_forward_slashes(const fs::path& path)
{
    std::string path_str = to_utf8_path(path);
    std::replace(path_str.begin(), path_str.end(), '\\', '/');
    return path_str;
}


boost::optional<std::string> get_executable_path()
{
#if BOOST_OS_WINDOWS
    TCHAR buf_wide[1024 + 1];
    size_t buf_size = sizeof(buf_wide);
    if (GetModuleFileName(nullptr, buf_wide, buf_size) == 0)
    {
        return boost::none;
    }
    char buf[2048 + 1];
    wcstombs(buf, buf_wide, wcslen(buf_wide) + 1);
#elif BOOST_OS_MACOS
    char buf[PATH_MAX + 1];
    uint32_t buf_size = sizeof(buf);
    if (_NSGetExecutablePath(buf, &buf_size) != 0)
    {
        return boost::none;
    }
#elif BOOST_OS_LINUX
    char buf[PATH_MAX + 1];
    size_t buf_size = sizeof(buf);
    ssize_t result = readlink("/proc/self/exe", buf, buf_size);
    if (result == -1)
    {
        return boost::none;
    }
    buf[result] = '\0';
#elif BOOST_OS_ANDROID
    std::string external_storage_path(SDL_AndroidGetExternalStoragePath());
    if (external_storage_path.back() != '/')
        external_storage_path += '/';
    const char* buf = external_storage_path.c_str();
#else
#error Unsupported OS
#endif

    return std::string(buf);
}

boost::optional<std::string> get_home_directory()
{
    boost::optional<std::string> home_directory;
#if BOOST_OS_WINDOWS
    wchar_t* knownFolderPath;

    auto hr = SHGetKnownFolderPath(
        FOLDERID_Profile, KF_FLAG_DEFAULT, nullptr, &knownFolderPath);

    if (SUCCEEDED(hr))
    {
        char result[MAX_PATH + 1];
        wcstombs(result, knownFolderPath, MAX_PATH);
        home_directory = std::string(result);
        CoTaskMemFree(knownFolderPath);
    }
#elif BOOST_OS_LINUX || BOOST_OS_MACOS
    char* result = getenv("HOME");
    if (result == nullptr)
    {
        struct passwd* pwd = getpwuid(getuid());
        if (pwd)
        {
            result = pwd->pw_dir;
        }
    }
    if (result != nullptr)
    {
        home_directory = result;
    }
#endif
    return home_directory;
}

} // namespace filepathutil
