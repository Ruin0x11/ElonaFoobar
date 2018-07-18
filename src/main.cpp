#include <iostream>
#include <stdexcept>
#include <SDL.h>
#include "asura/asura.hpp"
#include "defines.hpp"
#include "init.hpp"
#include "log.hpp"
#include "version.hpp"
#if defined(ELONA_OS_WINDOWS)
#include <windows.h> // OutputDebugStringA
#endif

namespace elona
{

void report_error(const char* what)
{
#if defined(ELONA_OS_WINDOWS)
    OutputDebugStringA(what);
#elif defined(ELONA_OS_ANDROID)
    LOGD("Error: %s", what);
#endif
    asura::dialog::state state = {"A fatal error has occurred:", std::string(what)};
    asura::dialog::ok(state,
                      asura::dialog::message_type::error);

    ELONA_LOG("Error: " << what);
    std::cerr << "Error: " << what << std::endl;
}
} // namespace elona



int main(int argc, char** argv)
{
    using namespace elona;
    (void)argc, (void)argv;

    asura::app::init();

    log::initialize();

    ELONA_LOG(latest_version.long_string());

    try
    {
        asura::dialog::state state = {"yn"};
        asura::dialog::yes_or_no(state, asura::dialog::message_type::warning);
        throw std::runtime_error(std::string("asd"));
        return run();
    }
    catch (std::exception& e)
    {
        report_error(e.what());
    }

    asura::app::quit();

    return 0;
}
