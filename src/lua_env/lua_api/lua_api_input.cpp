#include "lua_api_input.hpp"
#include "../../i18n.hpp"
#include "../../input.hpp"
#include "../../input_prompt.hpp"
#include "../../message.hpp"

namespace elona
{
namespace lua
{

bool Input::yes_no(const std::string& message)
{
    keyhalt = 1;
    txt(message + i18n::space_if_needed());
    rtval = yes_or_no(promptx, prompty, 160);
    return rtval == 0;
}

sol::optional<int> Input::prompt_number(const std::string& message, int max)
{
    return Input::prompt_number_with_initial(message, max, 0);
}

sol::optional<int> Input::prompt_number_with_initial(
    const std::string& message,
    int max,
    int initial)
{
    if (max < 1)
    {
        throw sol::error("Input.prompt_number called with max < 1");
    }

    keyhalt = 1;
    txt(message + i18n::space_if_needed());
    input_number_dialog(
        (windoww - 200) / 2 + inf_screenx, winposy(60), max, initial);

    if (rtval == -1)
    {
        return sol::nullopt;
    }

    return elona::stoi(elona::inputlog(0));
}

sol::optional<std::string> Input::prompt_text(
    const std::string& message,
    bool is_cancelable)
{
    txt(message + i18n::space_if_needed());
    bool canceled = input_text_dialog(
        (windoww - 360) / 2 + inf_screenx,
        winposy(90),
        20,
        is_cancelable,
        true);
    if (canceled)
    {
        return sol::nullopt;
    }

    return elona::inputlog(0);
}

sol::optional<int> Input::prompt_choice(sol::variadic_args args)
{
    if (args.size() == 0)
    {
        throw sol::error("Input.prompt_choice called with no arguments");
    }

    Prompt prompt;
    size_t width = 160;
    for (size_t i = 0; i < args.size(); i++)
    {
        auto text = args[i].as<std::string>();
        width = std::max(width, strlen_u(text) * (13 - en * 2));
        prompt.append(std::move(text), i);
    }

    keyhalt = 1;
    int rtval = prompt.query(promptx, prompty, (int)width);
    if (rtval == -1)
    {
        return sol::nullopt;
    }

    // Lua tables are 1-indexed, so add 1 to the result.
    return rtval + 1;
}

sol::optional<Position> Input::prompt_position(const std::string& message)
{
    return Input::prompt_position_with_initial_xy(message, 0, 0);
}

sol::optional<Position> Input::prompt_position_with_initial(
    const std::string& message,
    const Position& pos)
{
    return Input::prompt_position_with_initial_xy(message, pos.x, pos.y);
}

sol::optional<Position>
Input::prompt_position_with_initial_xy(const std::string& message, int x, int y)
{
    elona::tlocinitx = x;
    elona::tlocinity = y;

    txt(message + i18n::space_if_needed());

    int result = elona::target_position(false);

    if (result == -1)
    {
        return sol::nullopt;
    }

    return Position{tlocx, tlocy};
}

bool Input::any_key_pressed()
{
    return snail::Input::instance().pressed_keys().size() > 0;
}

void Input::bind(sol::table& api_table)
{
    LUA_API_BIND_FUNCTION(api_table, Input, yes_no);
    LUA_API_BIND_FUNCTION(api_table, Input, prompt_choice);
    api_table.set_function(
        "prompt_number",
        sol::overload(Input::prompt_number, Input::prompt_number_with_initial));
    LUA_API_BIND_FUNCTION(api_table, Input, prompt_text);
    api_table.set_function(
        "prompt_position",
        sol::overload(
            Input::prompt_position,
            Input::prompt_position_with_initial,
            Input::prompt_position_with_initial_xy));
    LUA_API_BIND_FUNCTION(api_table, Input, any_key_pressed);
}

} // namespace lua
} // namespace elona
