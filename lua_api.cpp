#include "lua.hpp"

namespace elona
{

namespace lua
{

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
    elona::cell_check(pos.x, pos.y);
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

void init_api(sol::state& state)
{
    state.get()->new_usertype<position_t>( "position",
                                         sol::constructors<position_t()>()
        );
    state.get()->new_usertype<character>( "character",
                                        sol::constructors<character()>(),
                                        "damage_hp", &Chara::mut_damage_hp,
                                        "damage_con", &Chara::mut_damage_con,
                                        "idx", sol::readonly( &character::idx )
        );

    sol::table Elona = state.get()->create_named_table("Elona");
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
}


} // name lua

} // namespace elona
