#include "lua_api_debug.hpp"
#include <sstream>
#include "../../character.hpp"
#include "../../enums.hpp"
#include "../../item.hpp"
#include "../../log.hpp"
#include "../../message.hpp"



namespace elona
{
namespace lua
{

/**
 * @luadoc
 *
 * Logs a message to log.txt.
 * @tparam string message the message to log
 * @tparam[opt] string tag the log tag
 * @tparam[opt] Enums.LogLevel level the log level
 */
void LuaApiDebug::log(
    const std::string& message,
    sol::optional<std::string> tag,
    sol::optional<EnumString> level)
{
    auto tag_value = tag.value_or("lua.log");
    auto level_value = log::Logger::Level::info;
    if (level)
    {
        level_value = LuaEnums::LogLevelTable.ensure_from_string(*level);
    }

    switch (level_value)
    {
    case log::Logger::Level::info: ELONA_LOG(tag_value) << message; break;
    case log::Logger::Level::warn: ELONA_WARN(tag_value) << message; break;
    case log::Logger::Level::error: ELONA_ERROR(tag_value) << message; break;
    case log::Logger::Level::fatal: ELONA_FATAL(tag_value) << message; break;
    }
}

/**
 * @luadoc
 *
 * Reports an error to the game console and log file.
 * @tparam string message the error message to report
 */
void LuaApiDebug::report_error(const std::string& message)
{
    std::istringstream sstream(message);
    std::string line;

    txt("Script error: ", Message::color{ColorIndex::red});
    while (getline(sstream, line, '\n'))
    {
        txt(line + "  ", Message::color{ColorIndex::red});
    }

    ELONA_ERROR("lua.debug") << "Script error: " << message;
    std::cerr << "Script error: " << message << std::endl;
}

void LuaApiDebug::bind(sol::table& api_table)
{
    LUA_API_BIND_FUNCTION(api_table, LuaApiDebug, log);
    LUA_API_BIND_FUNCTION(api_table, LuaApiDebug, report_error);
}

} // namespace lua
} // namespace elona
