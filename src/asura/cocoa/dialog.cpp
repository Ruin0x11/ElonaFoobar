namespace dialog
{

enum class buttons
{
    ok,
    yes_no,
};

static dialog::response dialog(dialog::state& state,
                        dialog::message_type message_type,
                        buttons buttons)
{
    @autoreleasepool
    {
        NSAlert* alert = [[[NSAlert alloc] init] autorelease];
        [alert setMessageText:[NSString stringWithUTF8String:state.message.c_str()]];

        if (state.submessage() != "")
            [alert setInformativeText:[NSString stringWithUTF8String:state.submessage.c_str()]];

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
        case dialog::message_type::information:
            [alert setAlertStyle:NSAlertStyleInformational];
            break;
        case dialog::message_type::warning:
            [alert setAlertStyle:NSAlertStyleWarning];
            break;
        case dialog::message_type::error:
            [alert setAlertStyle:NSAlertStyleCritical];
            break;
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

dialog::response ok(dialog::state& state, dialog::message_type message_type)
{
    return dialog(state, message_type, buttons::ok);
}

dialog::response yes_or_no(dialog::state& state, dialog::message_type message_type)
{
    return dialog(state, message_type, buttons::yes_no);
}

}
