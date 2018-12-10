#include "lua_api_input.hpp"
#include "../../i18n.hpp"
#include "../../input.hpp"
#include "../../input_prompt.hpp"
#include "../../message.hpp"
#include "../../talk.hpp"

namespace elona
{
namespace lua
{

bool Input::yes_no(const std::string& message)
{
    txt(message + " ");
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

    txt(message + " ");
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
    txt(message + " ");
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

sol::optional<int> Input::prompt_choice(sol::table choices)
{
    Prompt prompt;

    // Lua tables are 1-indexed, but the prompt is 0-indexed.
    for (size_t i = 1; i <= choices.size(); i++)
    {
        std::string choice = choices[i];
        prompt.append(choice, i - 1);
    }

    int rtval = prompt.query(promptx, prompty, 240);
    if (rtval == -1)
    {
        return sol::nullopt;
    }

    // Lua tables are 1-indexed, so add 1 to the result.
    return rtval + 1;
}


static void _append_choices(sol::table& choices)
{
    int index = 0;
    std::string text;

    for (size_t i = 1; i <= choices.size(); i++)
    {
        sol::object value = choices[i];

        if (value.is<std::string>())
        {
            text = value.as<std::string>();
        }
        else if (value.is<sol::table>())
        {
            sol::table choice = value.as<sol::table>();
            index = choice.get<int>("index");
            text = choice.get<std::string>("text");
        }
        else
        {
            throw sol::error{
                "Dialog choices must be text or a table like "
                "{ index = 1, text = \"some text\" }"};
        }

        ELONA_APPEND_RESPONSE(index, text);
        index++;
    }
}

sol::optional<int> Input::prompt_dialog(
    const std::string& text,
    const std::string& portrait_id,
    const std::string& speaker_name,
    sol::table choices)
{
    // Copy text, as it is used mutably.
    auto text_ = text;

    _append_choices(choices);

    talk_start();
    auto result =
        talk_window_query(portrait_id, none, speaker_name, text_, none);

    if (result == -1)
    {
        return sol::nullopt;
    }

    return result;
}

sol::optional<int> Input::prompt_dialog_impress(
    const std::string& text,
    const std::string& portrait_id,
    const std::string& speaker_name,
    int impression,
    int interest,
    sol::table choices)
{
    // Copy text, as it is used mutably.
    auto text_ = text;

    auto impress_interest = std::make_pair(impression, interest);

    _append_choices(choices);

    talk_start();
    auto result = talk_window_query(
        portrait_id, none, speaker_name, text_, impress_interest);

    if (result == -1)
    {
        return sol::nullopt;
    }

    return result;
}

sol::optional<int> Input::prompt_dialog_with_chip(
    const std::string& text,
    int chara_image,
    const std::string& speaker_name,
    sol::table choices)
{
    // Copy text, as it is used mutably.
    auto text_ = text;

    _append_choices(choices);

    talk_start();
    auto result =
        talk_window_query(none, chara_image, speaker_name, text_, none);

    if (result == -1)
    {
        return sol::nullopt;
    }

    return result;
}

sol::optional<int> Input::prompt_dialog_with_chip_impress(
    const std::string& text,
    int chara_image,
    const std::string& speaker_name,
    int impression,
    int interest,
    sol::table choices)
{
    // Copy text, as it is used mutably.
    auto text_ = text;

    auto impress_interest = std::make_pair(impression, interest);

    talk_start();
    _append_choices(choices);

    auto result = talk_window_query(
        none, chara_image, speaker_name, text_, impress_interest);

    if (result == -1)
    {
        return sol::nullopt;
    }

    return result;
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
        "prompt_dialog",
        sol::overload(
            Input::prompt_dialog,
            Input::prompt_dialog_impress,
            Input::prompt_dialog_with_chip,
            Input::prompt_dialog_with_chip_impress));
}

} // namespace lua
} // namespace elona
