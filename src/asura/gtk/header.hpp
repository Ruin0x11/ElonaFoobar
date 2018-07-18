#include <cstring>

#define GTK_DISABLE_DEPRECATED

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wparentheses"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wparentheses"
#endif

#include <gtk/gtk.h>

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
