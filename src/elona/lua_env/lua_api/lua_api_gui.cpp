#include "lua_api_gui.hpp"
#include "../../audio.hpp"
#include "../../i18n.hpp"
#include "../../lua_env/enums/enums.hpp"
#include "../../message.hpp"
#include "../../quest.hpp"
#include "../../ui.hpp"
#include "lua_api_i18n.hpp"

namespace elona
{
namespace lua
{

/**
 * @luadoc
 *
 * Prints some text in the HUD message window. <code>message</code> can be a
 * localization key or a raw string.
 * @tparam string message the message to print
 */
void LuaApiGUI::txt(const std::string& message, sol::variadic_args rest)
{
    elona::txt(LuaApiI18N::get_or_pass(message, rest));
}

/**
 * @luadoc
 *
 * Prints some text in the HUD message window, with coloring. In this case, the
 * localization arguments start after the color argument.
 * @tparam string message the message to print
 * @tparam Color color of the message
 */
void LuaApiGUI::txt_colored(
    const std::string& message,
    const EnumString& color,
    sol::variadic_args rest)
{
    auto color_value = LuaEnums::ColorIndexTable.ensure_from_string(color);
    elona::Message::instance().txtef(color_value);
    LuaApiGUI::txt(message, rest);
}

/**
 * @luadoc
 *
 * Starts a new message line in the HUD message window.
 */
void LuaApiGUI::txtnew()
{
    elona::Message::instance().linebreak();
}

/**
 * @luadoc
 *
 * Plays a sound.
 * @tparam string sound_id The sound, of type core.sound
 */
void LuaApiGUI::play_sound(const std::string& sound_id)
{
    elona::snd(SharedId(sound_id));
}

/**
 * @luadoc
 *
 * Displays the journal update message.
 */
void LuaApiGUI::show_journal_update_message()
{
    quest_update_journal_msg();
}

/**
 * @luadoc
 *
 * Fades out the screen, then fades it in.
 */
void LuaApiGUI::fade_out()
{
    update_screen();
    elona::fade_out();
    gsel(0);
}

void LuaApiGUI::bind(sol::table& api_table)
{
    LUA_API_BIND_FUNCTION(api_table, LuaApiGUI, txt);
    LUA_API_BIND_FUNCTION(api_table, LuaApiGUI, txt_colored);
    LUA_API_BIND_FUNCTION(api_table, LuaApiGUI, txtnew);
    LUA_API_BIND_FUNCTION(api_table, LuaApiGUI, play_sound);
    LUA_API_BIND_FUNCTION(api_table, LuaApiGUI, show_journal_update_message);
    LUA_API_BIND_FUNCTION(api_table, LuaApiGUI, fade_out);
}

} // namespace lua
} // namespace elona
