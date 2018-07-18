namespace dialog
{

static GtkMessageType get_message_type(dialog::message_type message_type)
{
    switch(message_type)
    {
    case dialog::message_type::warning:
        return GTK_MESSAGE_WARNING;
    case dialog::message_type::error:
        return GTK_MESSAGE_ERROR;
    case dialog::message_type::information:
    default:
        return GTK_MESSAGE_INFO;
    }
}

dialog::response ok(const std::string& message, dialog::message_type message_type)
{
    GtkMessageType type = get_message_type(message_type);

    GtkWidget* dialog = gtk_message_dialog_new(
        NULL, GTK_DIALOG_MODAL, type, GTK_BUTTONS_OK,
        message.c_str());

    gtk_message_dialog_format_secondary_text(
        GTK_MESSAGE_DIALOG(dialog), "astring");

    gtk_dialog_run(GTK_DIALOG(dialog));

    return dialog::response::ok;
}

dialog::response yes_or_no(const std::string& message, dialog::message_type message_type)
{
    GtkMessageType type = get_message_type(message_type);

    GtkWidget* dialog = gtk_message_dialog_new(
        NULL, GTK_DIALOG_MODAL, type, GTK_BUTTONS_YES_NO,
        message.c_str());

    gtk_message_dialog_format_secondary_text(
        GTK_MESSAGE_DIALOG(dialog), "astring");

    auto response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_YES)
    {
        return dialog::response::yes;
    }
    else
    {
        return dialog::response::no;
    }
}

}

