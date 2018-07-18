namespace dialog
{

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

dialog::response ok(dialog::state& state, dialog::message_type message_type)
{
    std::string message = state.message;
    if (state.submessage != "")
        message += "\n" + state.submessage;

    std::wstring message_wstr = utf8_to_utf16(message);
    UINT type = get_message_type(message_type);

    MessageBoxW(NULL, message_wstr.c_str(), L"Message", MB_OK | type);

    return dialog::response::ok;
}

dialog::response yes_or_no(dialog::state& state, dialog::message_type message_type)
{
    std::string message = state.message;
    if (state.submessage != "")
        message += "\n" + state.submessage;

    std::wstring message_wstr = utf8_to_utf16(message);
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
