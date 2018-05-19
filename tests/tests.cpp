#define CATCH_CONFIG_RUNNER
#include "../thirdparty/catch/catch.hpp"

int main(int argc, char* argv[])
{
    srand(static_cast<unsigned int>(std::time(0)));

    return Catch::Session().run(argc, argv);
}
