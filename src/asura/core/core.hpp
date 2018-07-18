#pragma once
#if defined(ELONA_OS_WINDOWS)
#include "../windows/header.hpp"
#elif defined(ELONA_OS_MACOS)
#include "../cocoa/header.hpp"
#elif defined(ELONA_OS_LINUX)
#include "../gtk/header.hpp"
#else
#include "../reference/header.hpp"
#endif

namespace elona
{
namespace asura
{

#include "app.hpp"
#include "dialog.hpp"

}
}
