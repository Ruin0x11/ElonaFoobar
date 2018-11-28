#include "lua_api_macro.hpp"
#include "../../keybind/macro_action_queue.hpp"

namespace elona
{
namespace lua
{

void Macro::enqueue(const std::string& action)
{
    keybind::macro_action_queue.push(action);
}

void Macro::enqueue_table(sol::table table)
{
    for (size_t i = 1; i <= table.size(); i++)
    {
        sol::optional<std::string> action = table[i];
        if (action)
        {
            enqueue(*action);
        }
    }
}

void Macro::clear_queue()
{
    keybind::macro_action_queue.clear();
}

void Macro::bind(sol::table& api_table)
{
    api_table.set_function(
        "enqueue", sol::overload(Macro::enqueue, Macro::enqueue_table));
    LUA_API_BIND_FUNCTION(api_table, Macro, clear_queue);
}

} // namespace lua
} // namespace elona
