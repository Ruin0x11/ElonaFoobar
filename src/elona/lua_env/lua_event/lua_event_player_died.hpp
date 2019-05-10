#pragma once
#include "../interface.hpp"
#include "base_event.hpp"

namespace elona
{
namespace lua
{

struct PlayerDiedEvent : BaseEvent
{
public:
    PlayerDiedEvent(
        std::string map_name,
        std::string death_cause,
        std::string last_words,
        bool buried)
        : BaseEvent("core.player_died")
        , _map_name(map_name)
        , _death_cause(death_cause)
        , _last_words(last_words)
        , _buried(buried)
    {
    }

    sol::table make_event_table() const override
    {
        auto args = lua::create_table();
        args["map_name"] = _map_name;
        args["death_cause"] = _death_cause;
        args["last_words"] = _last_words;
        args["buried"] = _buried;

        return args;
    };

private:
    std::string _map_name;
    std::string _death_cause;
    std::string _last_words;
    bool _buried;
};

} // namespace lua
} // namespace elona
