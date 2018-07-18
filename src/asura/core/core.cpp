#include "core.hpp"

namespace elona
{
namespace asura
{

#if defined(ELONA_OS_WINDOWS)
#include "../windows/platform.cpp"
#elif defined(ELONA_OS_MACOS)
#include "../cocoa/platform.cpp"
#elif defined(ELONA_OS_LINUX)
#include "../gtk/platform.cpp"
#else
#include "../reference/platform.cpp"
#endif

}
}
