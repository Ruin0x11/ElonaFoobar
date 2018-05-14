#include "lua.hpp"
#include "map.hpp"
#include "elona.hpp"
#include "character.hpp"
#include "item.hpp"
#include "position.hpp"
#include "variables.hpp"
#include "log.hpp"
#include <vector>
#include <map>

namespace elona
{
namespace lua
{

// TODO wrap all table editing methods in their own functions
// TODO make everything less messy
// TODO move api to separate source file
// TODO make things immutable where they need to be

std::unique_ptr<sol::state> sol;

namespace Chara {
bool is_alive(const character&);
bool is_player(const character&);
bool is_ally(const character&);
character player();

void mut_damage_hp(character&, int, int);
void mut_damage_con(character&, int, int);
};

namespace Skill {
//int level();
}

namespace Pos {
int dist(const position_t&, const position_t&);
}

namespace World {
int time();
};

namespace Magic {
void cast(int, int, const position_t&);
}

namespace Map {
int blocked(const position_t&);
position_t bound_within(const position_t&);
bool can_access(const position_t&);
position_t random_pos();
}

namespace Fov {
bool los(const position_t&, const position_t&);
bool can_see(const character&);
};

namespace Rand {
int rnd(int);
bool one_in(int);
bool coinflip();
};

namespace Item {
bool has_enchantment(const item&, int);
}

namespace GUI {
void txt(const std::string&);
};

namespace Registry {
void set_on_event(const std::string&, const sol::function&);
void register_chara_init(const sol::function&);
}


bool Chara::is_alive(const character& chara)
{
    return chara.state == 1;
}

bool Chara::is_player(const character& chara)
{
    return chara.id == 0; // TODO
}

bool Chara::is_ally(const character& chara)
{
    return chara.id <= 16; // TODO
}

character Chara::player()
{
    return elona::cdata[0];
}

void Chara::mut_damage_hp(character& chara, int damage, int type)
{
    elona::dmghp(chara.id, damage, type); // TODO
}

void Chara::mut_damage_con(character& chara, int damage, int type)
{
    elona::dmgcon(chara.id, damage, type); // TODO
}


int Pos::dist(const position_t& from, const position_t& to)
{
    return elona::dist(from.x, from.y, to.x, to.y);
}


int World::time()
{
    return gdata_hour
        + gdata_day * 24
        + gdata_month * 24 * 30
        + gdata_year * 24 * 30 * 12;
}


void Magic::cast(int efid, int efp, const position_t& pos)
{
    int ccbk = elona::cc;
    int tcbk = elona::tc;
    elona::cc = 0;
    elona::tc = 0;
    elona::efid = efid;
    elona::efp = efp;
    elona::tlocx = pos.x;
    elona::tlocy = pos.y;
    magic();
    elona::cc = ccbk;
    elona::tc = tcbk;
}



int Map::blocked(const position_t& pos)
{
    return elona::map(pos.x, pos.y, 0) == 0;
}

position_t Map::bound_within(const position_t& pos)
{
    int x = clamp(
        pos.x,
        0,
        mdata(0) - 1);
    int y = clamp(
        pos.y,
        0,
        mdata(1) - 1);
    return position_t{x, y};
}

bool Map::can_access(const position_t& pos)
{
    cell_check(pos.x, pos.y);
    return cellaccess != 0;
}

position_t Map::random_pos()
{
    return Map::bound_within(
        position_t{
            rnd(mdata(0) - 1),
                rnd(mdata(1) - 1)
                });
}


bool Fov::los(const position_t& from, const position_t& to)
{
    return elona::fov_los(from.x, from.y, to.x, to.y) == 1;
}

bool Fov::can_see(const character& chara)
{
    return elona::is_in_fov(chara.id); // TODO
}


int Rand::rnd(int n)
{
    return elona::rnd(n);
}

bool Rand::one_in(int n)
{
    return elona::rnd(n) == 0;
}

bool Rand::coinflip()
{
    return elona::rnd(2) == 0;
}


bool Item::has_enchantment(const item& item, int id)
{
    return elona::encfindspec(item.id, id); //TODO
}


void GUI::txt(const std::string& str)
{
    elona::txt(str);
}

void reload()
{
    // TODO more sophisticated reloading
    (*sol.get())["Global"]["Callbacks"] = sol::nullopt;

    load_mod("core");
    txt("Reloaded core/init.lua. ");
}

void load_mod(const std::string& name)
{
    (*sol.get())["Global"]["MOD_NAME"] = name;
    // create character/item/map/global tables

    // TODO this could overwrite a thing
    sol::table data = (*sol.get())["Elona"]["Registry"]["Data"];

    sol::table modlocal = data.create_named(name);

    sol::table Chara = modlocal.create_named("Chara");
    sol::table Item = modlocal.create_named("Item");
    sol::table Map = modlocal.create_named("Map");
    sol::table Global = modlocal.create_named("Global");
    // run various mod loading stages (like defining custom fields for all prototypes in the game?)
    // evaluate init.lua to load defines
    sol.get()->script_file("mods/"s + name + "/init.lua"s);
    // determine mod overrides inside .json files
    // merge overrides, new things, and locale configs into global database
    // add reference to global API table as Elona so the mod can use it
    (*sol.get())["Global"]["MOD_NAME"] = "";
}

void Registry::set_on_event(const std::string& event_id, const sol::function& func)
{
    std::string mod_name = (*sol.get())["Global"]["MOD_NAME"];
    ELONA_LOG("Setting " << event_id << " of " << mod_name);
    (*sol.get())["Global"]["Callbacks"][event_id] = func;
    ELONA_LOG("Set " << event_id << " of " << mod_name);
}

void Registry::register_chara_init(const sol::function& func)
{
    (*sol.get())["Global"]["Init"] = func;
}

void callback(const std::string& event_id)
{
    callback(event_id, {});
}

void callback(const std::string& event_id, const std::map<std::string, int> args)
{
    // for each mod, if there is an array of callbacks at the given name in its exports, run all of them in order
    // could use hashset to determine quickly which mods define callbacks
    // or, use a plain list with references to the exported callbacks of each mod
    sol::optional<sol::protected_function> func = (*sol.get())["Global"]["Callbacks"][event_id];
    if (func && func.value() != sol::nil)
    {
        auto result = func.value()(event_id, args);
        if (!result.valid())
        {
            txtef(3);
            sol::error err = result;
            std::string what = err.what();
            ELONA_LOG(what);
            txt(what);
        }
    }
}
//
// sol::any call(const std::string& mod_name, const std::string& func_name)
// {
//     // validate the mod exists
//     // try to see if the mod's exports define the named function
//     // if so, call it and marshal its result back to the caller
//     // if not, return nil
// }
//
// int formula(const std::string& id)
// {
//     // use a hashmap to look up which mod has the formula, then call it and return the result
//     // TODO think about performance overhead of having multiple mods run formulas in turn
//     return 0;
// }
//
// void on_save()
// {
//     // for each mod, serialize everything
       // TODO if character/item reference, serialize as integer id
// }
//
// void on_load()
// {
//     // for each mod, deserialize everything
// }
//
// void on_map_initialize()
// {
//     // for each mod, deserialize its map/chara/item tables from the new map, or initialize if nonexisting
//     // run mod callbacks for map initialization
// }
//
// void on_map_exit()
// {
//     // for each mod, serialize its map/chara/item tables
//     // run mod callbacks for map exit
// }
//
void on_chara_creation(int id)
{
    // TODO handle deserialization separately from creation from scratch
    // TODO only handle deserialization for characters that actually exist
    // for each mod, init its extra data for the character
    // for each mod, run chara creation callback

    //callback("chara_created", {{"cc", id}});

    sol::table data = (*sol.get())["Elona"]["Registry"]["Data"];
    sol::optional<sol::protected_function> func = (*sol.get())["Global"]["Init"];
     for(const auto& obj : data)
    {
         const std::string key = obj.first.as<std::string>();
         if(func && func.value() != sol::nil) {
             auto result = func.value()(id); // TODO except player/allies/respawnable characters
             if (result.valid())
             {
                 data[key]["Chara"][id] = result;
             }
             else
             {
                 sol::error err = result;
                 std::string what = err.what();
                 ELONA_LOG(what);
             }
         }
     }
}
//
// void on_item_creation(int id)
// {
//     // for each mod, init its extra data for the item
//     // for each mod, run item creation callback
// }
//
void on_chara_removal(int id)
{
    ELONA_LOG("Character removed. Here is the data that was lost.\n")
    sol::table data = (*sol.get())["Elona"]["Registry"]["Data"];
    for(auto& obj : data)
    {
        data[obj]["Chara"][id] = sol::nullopt; // TODO except player/allies/respawnable characters
    }

    // for each mod, invalidate global chara state
    // for each mod, run chara removal callback
}
//
// void on_item_removal(int id)
// {
//     // for each mod, invalidate global item state
//     // for each mod, run item removal callback
// }

void init()
{
    sol = std::make_unique<sol::state>();
    sol.get()->open_libraries(sol::lib::base, sol::lib::package, sol::lib::table, sol::lib::debug, sol::lib::string);
    sol.get()->new_usertype<position_t>( "position",
                                         sol::constructors<position_t()>()
        );
    sol.get()->new_usertype<character>( "character",
                                        sol::constructors<character()>(),
                                        "damage_hp", &Chara::mut_damage_hp,
                                        "damage_con", &Chara::mut_damage_con,
                                        "idx", sol::readonly( &character::idx )
        );

    sol::table Elona = sol.get()->create_named_table("Elona");
    Elona.set_function("log", [](const std::string& msg) { elona::log::detail::out << msg << std::endl; } );

    sol::table Chara = Elona.create_named("Chara");
    Chara.set_function("player", Chara::player);

    sol::table Fov = Elona.create_named("Fov");
    Fov.set_function("los", Fov::los);

    sol::table Rand = Elona.create_named("Rand");
    Rand.set_function("rnd", Rand::rnd);
    Rand.set_function("one_in", Rand::one_in);

    sol::table Magic = Elona.create_named("Magic");
    Magic.set_function("cast", Magic::cast);

    sol::table Map = Elona.create_named("Map");
    Map.set_function("random_pos", Map::random_pos);

    sol::table GUI = Elona.create_named("GUI");
    GUI.set_function("txt", GUI::txt);

    sol::table Registry = Elona.create_named("Registry");
    Registry.set_function("on_event", Registry::set_on_event);
    Registry.set_function("register_chara_init", Registry::register_chara_init);
    Registry.create_named("Data");

    sol::table Global = sol.get()->create_named_table("Global");
    Global.create_named("Callbacks");
    Global.create_named("Init");

    // prevent usage of some tables during mod loading, since calling things like GUI.txt at the top level before starting the game is dangerous
    // load core mod first
    load_mod("core");
    // for each other mod, load_mod()
    // after global prototype db is made, make it read-only
    // make stdlib table Elona read-only
}

} // namespace lua
} // namespace elona
