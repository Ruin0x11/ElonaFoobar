namespace dialog
{

dialog::response ok(dialog::state&, dialog::message_type)
{
    return dialog::response::ok;
}

dialog::response yes_or_no(dialog::state&, dialog::message_type)
{
    return dialog::response::yes;
}

}
