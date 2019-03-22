#pragma once
#include <chrono>
#include "../../log.hpp"
#include "../enums/enums.hpp"
#include "base_event.hpp"

namespace elona
{
namespace lua
{

struct LogMessageReceivedEvent : BaseEvent
{
private:
    using duration = std::chrono::duration<double>;

public:
    LogMessageReceivedEvent(
        std::string message,
        duration elapsed_time,
        std::string tag,
        log::Logger::Level level)
        : BaseEvent("core.log_message_received")
    {
        _message = message;
        _elapsed_time = elapsed_time;
        _level = level;
        _tag = tag;
    }

    sol::table make_event_table() const override
    {
        auto args = lua::create_table();

        args["message"] = _message;
        args["timestamp"] = _elapsed_time.count();
        args["tag"] = _tag;
        args["level"] = LuaEnums::LogLevelTable.convert_to_string(_level);

        return args;
    };

private:
    std::string _message;
    duration _elapsed_time;
    std::string _tag;
    log::Logger::Level _level;
};

} // namespace lua
} // namespace elona
