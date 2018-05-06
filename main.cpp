#include <stdexcept>
#include <SDL.h>
#include "log.hpp"
#include "version.hpp"
#include <iostream>
#include "defines.hpp"
#include "asura/asura.hpp"

namespace elona
{
int run();

void report_error(const char* what) {
    asura::dialog::ok(std::string(what), asura::dialog::message_type::error);
    ELONA_LOG(what);
}
}



int main(int argc, char** argv)
{
    using namespace elona;
    (void)argc, (void)argv;

    log::initialize();

    ELONA_LOG(latest_version.long_string());

    try
    {
        return run();
    }
    catch (std::exception& e)
    {
		report_error(e.what());
    }
    catch (...)
    {
		report_error(u8"Error occurred for some reason.");
    }
}
