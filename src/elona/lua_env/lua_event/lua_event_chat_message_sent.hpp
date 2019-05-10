#pragma once
#include "../interface.hpp"
#include "base_event.hpp"

namespace elona
{
namespace lua
{

struct ChatMessageSentEvent : BaseEvent
{
public:
    ChatMessageSentEvent(std::string message)
        : BaseEvent("core.chat_message_sent")
        , _message(message)
    {
    }

    sol::table make_event_table() const override
    {
        auto args = lua::create_table();
        args["message"] = _message;

        return args;
    };

private:
    std::string _message;
};

} // namespace lua
} // namespace elona
