#include "asura.hpp"
#include "../defines.hpp"

#if defined(ELONA_OS_WINDOWS)
#include "windows/header.hpp"
#elif defined(ELONA_OS_MACOS)
#include "cocoa/header.hpp"
#elif defined(ELONA_OS_LINUX)
#include "gtk/header.hpp"
#else
#include "reference/header.hpp"
#endif

namespace elona
{
namespace asura
{

#if defined(ELONA_OS_WINDOWS)
#include "windows/platform.cpp"
#elif defined(ELONA_OS_MACOS)
#include "cocoa/platform.cpp"
#elif defined(ELONA_OS_LINUX)
#include "gtk/platform.cpp"
#else
#include "reference/platform.cpp"
#endif

}
}


namespace elona
{
namespace asura
{

// Browser

std::string browser::directory()
{
    return browser_impl::directory();
}


// Dialog

dialog::response dialog::ok(const std::string& message, dialog::message_type message_type)
{
    return dialog_impl::ok(message, message_type);
}

dialog::response dialog::yes_or_no(const std::string& message, dialog::message_type message_type)
{
    return dialog_impl::yes_or_no(message, message_type);
}

}
}
