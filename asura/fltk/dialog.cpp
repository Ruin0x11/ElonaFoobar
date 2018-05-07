namespace dialog_impl
{

dialog::response ok(const std::string& message, dialog::message_type message_type)
{
    fl_choice(message.c_str(), "OK", nullptr, nullptr);
    return dialog::response::ok;
}

dialog::response yes_or_no(const std::string& message, dialog::message_type message_type)
{
    switch(fl_choice(message.c_str(), "Yes", "No", nullptr))
    {
        case 0:
            return dialog::response::yes;
        case 1:
        default:
            return dialog::response::no;
    }
}

}
