#include <iostream>
#include <stdexcept>
#include <sstream>
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
    std::ostringstream ss;
    ss << "A fatal error has occurred: " << std::endl;
    ss << std::string(what) << std::endl;
    asura::dialog::ok(ss.str(), asura::dialog::message_type::error);

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
