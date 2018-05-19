#define CATCH_CONFIG_RUNNER
#include "../thirdparty/catch/catch.hpp"
#include "../testing.hpp"

int main(int argc, char* argv[])
{
    srand(static_cast<unsigned int>(std::time(0)));

    elona::testing::pre_init();

    return Catch::Session().run(argc, argv);
}
