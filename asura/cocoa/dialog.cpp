namespace dialog_impl
{

enum class buttons
{
    ok,
    yes_no,
};

dialog::response dialog(const std::string& message, dialog::message_type message_type, buttons buttons)
{
    @autoreleasepool
    {
        NSAlert* alert = [[[NSAlert alloc] init] autorelease];
        [alert setInformativeText:[NSString stringWithUTF8String:message.c_str()]];

        switch(buttons) {
        case buttons::ok:
            [alert addButtonWithTitle:@"Ok"];
            break;
        case buttons::yes_no:
            [alert addButtonWithTitle:@"Yes"];
            [alert addButtonWithTitle:@"No"];
            break;
        }

        switch(message_type) {
        case dialog::message_type::information: [alert setAlertStyle:NSAlertStyleInformational]; break;
        case dialog::message_type::warning:     [alert setAlertStyle:NSAlertStyleWarning]; break;
        case dialog::message_type::error:       [alert setAlertStyle:NSAlertStyleCritical]; break;
        }

        NSInteger response = [alert runModal];

        switch(buttons) {
        case buttons::ok:
            return dialog::response::ok;
            break;
        case buttons::yes_no:
            if(response == NSAlertFirstButtonReturn)
                return dialog::response::yes;
            if(response == NSAlertSecondButtonReturn)
                return dialog::response::no;
            break;
        }

        return dialog::response::no;
    }
}

dialog::response ok(const std::string& message, dialog::message_type message_type)
{
    return dialog(message, message_type, buttons::ok);
}

dialog::response yes_or_no(const std::string& message, dialog::message_type message_type)
{
    return dialog(message, message_type, buttons::yes_no);
}

}
