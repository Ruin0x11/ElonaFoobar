#include "util.hpp"

static UINT get_message_type(dialog::message_type);

namespace dialog_impl
{

dialog::response ok(const std::string& message, dialog::message_type message_type)
{
    std::wstring message_wstr = utf8_to_utf_16(message);
    UINT type = get_message_type(message_type);

    MessageBoxW(NULL, message_wstr.c_str(), L"Message", MB_OK | type);

    return dialog::response::ok;
}

dialog::response yes_or_no(const std::string& message, dialog::message_type message_type)
{
    std::wstring message_wstr = utf8_to_utf_16(message);
    UINT type = get_message_type(message_type);

    UINT ret = MessageBoxW(NULL, message_wstr.c_str(), L"Message", MB_YESNO | type);
    if (ret == IDYES)
    {
        return dialog::response::yes;
    }
    else
    {
        return dialog::response::no;
    }
}

}


static UINT get_message_type(dialog::message_type message_type)
{
    switch(message_type)
    {
    case dialog::message_type::information:
        return MB_ICONINFORMATION;
    case dialog::message_type::warning:
        return MB_ICONWARNING;
    case dialog::message_type::error:
        return MB_ICONSTOP;
    }
}
