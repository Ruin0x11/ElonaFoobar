#pragma once
#include "../interface.hpp"
#include "base_event.hpp"

namespace elona
{
namespace lua
{

struct WishEndedEvent : BaseEvent
{
public:
    WishEndedEvent(std::string wish_text, std::string log_output)
        : BaseEvent("core.wish_ended")
        , _wish_text(wish_text)
        , _log_output(log_output)
    {
    }

    sol::table make_event_table() const override
    {
        auto args = lua::create_table();
        args["wish_text"] = _wish_text;
        args["log_output"] = _log_output;

        return args;
    };

private:
    std::string _wish_text;
    std::string _log_output;
};

} // namespace lua
} // namespace elona
