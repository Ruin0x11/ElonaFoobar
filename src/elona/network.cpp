#include "network.hpp"
#include "input.hpp"
#include "lua_env/event_manager.hpp"
#include "lua_env/lua_event/lua_event_chat_message_sent.hpp"
#include "message.hpp"
#include "variables.hpp"



namespace elona
{

void show_chat_dialog()
{
    inputlog = "";
    input_text_dialog(80, windowh - inf_verh - 70, 38);
    if (inputlog == ""s)
    {
        return;
    }
    if (jp)
    {
        inputlog = u8"「"s + inputlog + u8"」"s;
    }
    else
    {
        inputlog = u8"\""s + inputlog + u8"\""s;
    }
    txt(inputlog);
    lua::lua->get_event_manager().trigger(lua::ChatMessageSentEvent(inputlog));
}

} // namespace elona
