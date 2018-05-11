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
bool los(const position_t&, const position_t&);
};

namespace Rand {
int rnd(int);
};

namespace GUI {
void txt(const std::string&);
};


bool Fov::los(const position_t& from, const position_t& to)
{
    return elona::fov_los(from.x, from.y, to.x, to.y) == 1;
}


int Rand::rnd(int n)
{
    return elona::rnd(n);
}


void GUI::txt(const std::string& str)
{
    elona::txt(str);
}


void load_mod(const std::string& name)
{
    // create character/item/map/global tables
    // run various mod loading stages (like defining custom fields for all prototypes in the game?)
    // evaluate init.lua to load defines
    // determine mod overrides inside .json files
    // merge overrides, new things, and locale configs into global database
    // add reference to global API table as Elona so the mod can use it
}

void callback(const std::string& name)
{
    // for each mod, if there is an array of callbacks at the given name in its exports, run all of them in order
    // could use hashset to determine quickly which mods define callbacks
    // or, use a plain list with references to the exported callbacks of each mod
    // TODO handle varargs without using _G
}

sol::any call(const std::string& mod_name, const std::string& func_name)
{
    // validate the mod exists
    // try to see if the mod's exports define the named function
    // if so, call it and marshal its result back to the caller
    // if not, return nil
}

int formula(const std::string& id)
{
    // use a hashmap to look up which mod has the formula, then call it and return the result
    // TODO think about performance overhead of having multiple mods run formulas in turn
    return 0;
}

void on_save()
{
    // for each mod, serialize everything
}

void on_load()
{
    // for each mod, deserialize everything
}

void on_map_initialize()
{
    // for each mod, deserialize its map/chara/item tables from the new map, or initialize if nonexisting
    // run mod callbacks for map initialization
}

void on_map_exit()
{
    // for each mod, serialize its map/chara/item tables
    // run mod callbacks for map exit
}

void on_chara_creation(int id)
{
    // for each mod, init its extra data for the character
}

void on_item_creation(int id)
{
    // for each mod, init its extra data for the item
}

void on_chara_removal(int id)
{
    // for each mod, invalidate global chara state
}

void on_item_removal(int id)
{
    // for each mod, invalidate global item state
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

    // load core mod first
    // for each other mod, load_mod()
    // after global prototype db is made, make it read-only
    // make stdlib table Elona read-only
}

} // namespace lua
} // namespace elona
