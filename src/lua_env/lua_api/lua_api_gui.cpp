#include "lua_api_gui.hpp"
#include "../../i18n.hpp"
#include "../../lua_env/enums/enums.hpp"
#include "../../message.hpp"

namespace elona
{
namespace lua
{

void GUI::txt(const std::string& message)
{
    elona::txt(message + i18n::space_if_needed());
}

void GUI::txt_txtef(const std::string& message, const EnumString& color_name)
{
    GUI::txtef(color_name);
    elona::txt(message);
}

void GUI::txtef(const EnumString& color_name)
{
    ColorIndex color = LuaEnums::ColorIndexTable.ensure_from_string(color_name);
    elona::Message::instance().txtef(color);
}

void GUI::txtnew()
{
    elona::Message::instance().linebreak();
}

void GUI::bind(sol::table& api_table)
{
    api_table.set_function("txt", sol::overload(GUI::txt, GUI::txt_txtef));
    LUA_API_BIND_FUNCTION(api_table, GUI, txtef);
    LUA_API_BIND_FUNCTION(api_table, GUI, txtnew);
}

} // namespace lua
} // namespace elona
