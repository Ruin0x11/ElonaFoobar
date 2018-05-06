namespace dialog_impl
{

dialog::response ok(const std::string& message, dialog::message_type message_type)
{
    return dialog::response::ok;
}

dialog::response yes_or_no(const std::string& message, dialog::message_type message_type)
{
    return dialog::response::yes;
}

}
