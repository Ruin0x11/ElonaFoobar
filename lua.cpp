#include "lua.hpp"
#include "elona.hpp"
#include "position.hpp"
#include "variables.hpp"

namespace elona
{
namespace lua
{

std::unique_ptr<sol::state> sol;

namespace Fov {
bool los(position_t, position_t);
};

bool Fov::los(position_t from, position_t to)
{
    return elona::fov_los(from.x, from.y, to.x, to.y) == 1;
}

namespace Rand {
int rnd(int);
};

int Rand::rnd(int n)
{
    return elona::rnd(n);
}

namespace GUI {
void txt(const std::string&);
};

void GUI::txt(const std::string& str)
{
    elona::txt(str);
}

void init()
{
    sol = std::make_unique<sol::state>();
    sol.get()->new_usertype<position_t>( "position",
                                  sol::constructors<position_t()>()
        );

    sol::table Elona = sol.get()->create_named_table("Elona");

    sol::table Fov = Elona.create_named("Fov");
    Fov.set_function("los", Fov::los);

    sol::table Rand = Elona.create_named("Rand");
    Rand.set_function("rnd", Rand::rnd);

    sol::table GUI = Elona.create_named("GUI");
    GUI.set_function("txt", GUI::txt);
}

} // namespace lua
} // namespace elona
