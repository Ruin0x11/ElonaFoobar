#pragma once

namespace dialog_impl
{

dialog::response ok(const std::string& message, dialog::message_type message_type);
dialog::response yes_or_no(const std::string& message, dialog::message_type message_type);

}
