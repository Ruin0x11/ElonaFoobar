#include "lua.hpp"

#include "character.hpp"
#include "dmgheal.hpp"
#include "enchantment.hpp"
#include "fov.hpp"
#include "item.hpp"
#include "itemgen.hpp"
#include "log.hpp"
#include "map.hpp"
#include "map_cell.hpp"
#include "position.hpp"
#include "status_ailment.hpp"
#include "variables.hpp"

/***
 * See doc/api for the API documentation.
 * Be sure to update it if you change anything here.
 * LDoc is completely broken when it comes to parsing C++.
 */
namespace elona
{

namespace lua
{

namespace Chara {
bool is_alive(const character&);
bool is_player(const character&);
bool is_ally(const character&);
sol::optional<character*> player();
sol::optional<character*> create(const position_t&, int);
sol::optional<character*> create_xy(int, int, int);

void bind(sol::table& Elona);
};

bool Chara::is_alive(const character& character)
{
    return character.state == 1;
}

bool Chara::is_player(const character& character)
{
    return character.idx == 0;
}

bool Chara::is_ally(const character& character)
{
    return !Chara::is_player(character) && character.idx <= 16;
}

sol::optional<character*> Chara::player()
{
    if(elona::cdata[0].state == 0) {
        return sol::nullopt;
    }
    else
    {
        return &elona::cdata[0];
    }
}

sol::optional<character*> Chara::create(const position_t& position, int id)
{
    return Chara::create_xy(position.x, position.y, id);
}

sol::optional<character*> Chara::create_xy(int x, int y, int id)
{
    elona::flt();
    if(elona::chara_create(-1, id, x, y) != 0)
    {
        return &elona::cdata[elona::rc];
    }
    else
    {
        return sol::nullopt;
    }
}

void Chara::bind(sol::table& Elona)
{
    sol::table Chara = Elona.create_named("Chara");
    Chara.set_function("is_alive", Chara::is_alive);
    Chara.set_function("is_player", Chara::is_player);
    Chara.set_function("is_ally", Chara::is_ally);
    Chara.set_function("player", Chara::player);
    Chara.set_function("create", sol::overload(Chara::create, Chara::create_xy));
}

namespace Pos {
int dist(const position_t&, const position_t&);
int dist_xy(int, int, int, int);

void bind(sol::table& Elona);
}

int Pos::dist(const position_t& from, const position_t& to)
{
    return Pos::dist_xy(from.x, from.y, to.x, to.y);
}

int Pos::dist_xy(int fx, int fy, int tx, int ty)
{
    return elona::dist(fx, fy, tx, ty);
}

void Pos::bind(sol::table& Elona)
{
    sol::table Pos = Elona.create_named("Pos");
    Pos.set_function("dist", sol::overload(Pos::dist, Pos::dist_xy));
}


namespace World {
int time();
bool is_in_overworld();

void bind(sol::table& Elona);
};

int World::time()
{
    return gdata_hour
        + gdata_day * 24
        + gdata_month * 24 * 30
        + gdata_year * 24 * 30 * 12;
}

bool World::is_in_overworld()
{
    return elona::mdata(2) == 0;
}

void World::bind(sol::table& Elona)
{
    sol::table World = Elona.create_named("World");
    World.set_function("time", World::time);
}


namespace Magic {
void cast(int, int, const position_t&);

void bind(sol::table& Elona);
}

void Magic::cast(int effect_id, int effect_power, const position_t& target_location)
{
    int ccbk = elona::cc;
    int tcbk = elona::tc;
    elona::cc = 0;
    elona::tc = 0;
    elona::efid = effect_id;
    elona::efp = effect_power;
    elona::tlocx = target_location.x;
    elona::tlocy = target_location.y;
    elona::magic();
    elona::cc = ccbk;
    elona::tc = tcbk;
}

void Magic::bind(sol::table& Elona)
{
    sol::table Magic = Elona.create_named("Magic");
    Magic.set_function("cast", Magic::cast);
}

namespace Map {
int width();
int height();
bool valid(const position_t&);
bool valid_xy(int, int);
bool can_access(const position_t&);
bool can_access_xy(int, int);
position_t bound_within(const position_t&);
position_t random_pos();
void set_tile(const position_t&, tile_kind_t);
void set_tile_xy(int, int, tile_kind_t);
void set_tile_memory(const position_t&, tile_kind_t);
void set_tile_memory_xy(int, int, tile_kind_t);

void bind(sol::table& Elona);
}

int Map::width()
{
    return mdata(0);
}

int Map::height()
{
    return mdata(1);
}

bool Map::valid(const position_t& position)
{
    return Map::valid_xy(position.x, position.y);
}

bool Map::valid_xy(int x, int y)
{
    if (x < 0 || y < 0 || x >= Map::width() || y >= Map::height())
    {
        return false;
    }

    return elona::map(x, y, 0) != 0;
}

bool Map::can_access(const position_t& position)
{
    return Map::can_access_xy(position.x, position.y);
}

bool Map::can_access_xy(int x, int y)
{
    elona::cell_check(x, y);
    return cellaccess != 0;
}

position_t Map::bound_within(const position_t& position)
{
    int x = clamp(
        position.x,
        0,
        mdata(0) - 1);
    int y = clamp(
        position.y,
        0,
        mdata(1) - 1);
    return position_t{x, y};
}

position_t Map::random_pos()
{
    return Map::bound_within(
        position_t{
            elona::rnd(mdata(0) - 1),
                elona::rnd(mdata(1) - 1)
                });
}

void Map::set_tile(const position_t& position, tile_kind_t type)
{
    Map::set_tile_xy(position.x, position.y, type);
}

void Map::set_tile_xy(int x, int y, tile_kind_t type)
{
    if(!Map::valid_xy(x, y))
    {
        return;
    }

    elona::map(x, y, 0) = elona::cell_get_type(type);
}

void Map::set_tile_memory(const position_t& position, tile_kind_t type)
{
    Map::set_tile_memory_xy(position.x, position.y, type);
}

void Map::set_tile_memory_xy(int x, int y, tile_kind_t type)
{
    if(!Map::valid_xy(x, y))
    {
        return;
    }

    elona::map(x, y, 2) = elona::cell_get_type(type);
}

void Map::bind(sol::table& Elona)
{
    sol::table Map = Elona.create_named("Map");
    Map.set_function("width", Map::width);
    Map.set_function("height", Map::height);
    Map.set_function("valid", sol::overload(Map::valid, Map::valid_xy));
    Map.set_function("can_access", sol::overload(Map::can_access, Map::can_access_xy));
    Map.set_function("bound_within", Map::bound_within);
    Map.set_function("random_pos", Map::random_pos);
    Map.set_function("set_tile", sol::overload(Map::set_tile, Map::set_tile_xy));
    Map.set_function("set_tile_memory", sol::overload(Map::set_tile_memory, Map::set_tile_memory_xy));
}

namespace FOV {
bool los(const position_t&, const position_t&);
bool you_see(const character&);

void bind(sol::table& Elona);
};

bool FOV::los(const position_t& from, const position_t& to)
{
    return elona::fov_los(from.x, from.y, to.x, to.y) == 1;
}

bool FOV::you_see(const character& character)
{
    return elona::is_in_fov(character.idx);
}

void FOV::bind(sol::table& Elona)
{
    sol::table FOV = Elona.create_named("FOV");
    FOV.set_function("los", FOV::los);
    FOV.set_function("you_see", FOV::you_see);
}


namespace Rand {
int rnd(int n);
bool one_in(int n);
bool coinflip();

void bind(sol::table& Elona);
};


int Rand::rnd(int n)
{
    return elona::rnd(n);
}

bool Rand::one_in(int n)
{
    return Rand::rnd(n) == 0;
}

bool Rand::coinflip()
{
    return Rand::one_in(2);
}

void Rand::bind(sol::table& Elona)
{
    sol::table Rand = Elona.create_named("Rand");
    Rand.set_function("rnd", Rand::rnd);
    Rand.set_function("one_in", Rand::one_in);
    Rand.set_function("coinflip", Rand::coinflip);
}


namespace Item {
sol::optional<item*> create(const position_t&, int, int);
sol::optional<item*> create_xy(int, int, int, int);
bool has_enchantment(const item&, int);

void bind(sol::table& Elona);
}

sol::optional<item*> Item::create(const position_t& position, int id, int number)
{
    return Item::create_xy(position.x, position.y, id, number);
}

sol::optional<item*> Item::create_xy(int x, int y, int id, int number)
{
    elona::flt();
    if(elona::itemcreate(-1, id, x, y, number) != 0)
    {
        return &elona::inv[elona::ci]; // TODO deglobalize ci
    }
    else
    {
        return sol::nullopt;
    }
}

bool Item::has_enchantment(const item& item, int id)
{
    return elona::encfindspec(item.idx, id);
}

void Item::bind(sol::table& Elona)
{
    sol::table Item = Elona.create_named("Item");
    Item.set_function("create", sol::overload(Item::create, Item::create_xy));
}


namespace GUI {
void txt(const std::string& message);

void bind(sol::table& Elona);
};

void GUI::txt(const std::string& message)
{
    elona::txt(message);
}

void GUI::bind(sol::table& Elona)
{
    sol::table GUI = Elona.create_named("GUI");
    GUI.set_function("txt", GUI::txt);
}


namespace Debug
{
void log(const std::string&);
void dump_characters();
void dump_items();

void bind(sol::table& Elona);
}

void Debug::log(const std::string& message)
{
    ELONA_LOG(message);
}

void Debug::dump_characters()
{
    ELONA_LOG("===== Charas =====")
    for (int cnt = 0; cnt < ELONA_MAX_CHARACTERS; ++cnt)
    {
        if(elona::cdata[cnt].state != 0)
            ELONA_LOG(elona::cdata[cnt].idx << ") Name: " << elona::name(cnt) <<
                      ", Pos: " << elona::cdata[cnt].position);
    }
}

void Debug::dump_items()
{
    ELONA_LOG("===== Items  =====")
    for (const auto& cnt : items(-1))
    {
        if(elona::inv[cnt].number != 0)
            ELONA_LOG(elona::inv[cnt].idx <<") Name: " << elona::itemname(cnt) <<
                      ", Pos: "   << elona::inv[cnt].position <<
                      ", Curse: " << static_cast<int>(elona::inv[cnt].curse_state) <<
                      ", Ident: " << static_cast<int>(elona::inv[cnt].identification_state) <<
                      ", Count: " << elona::inv[cnt].count);
    }
}

void Debug::bind(sol::table& Elona)
{
    sol::table Debug = Elona.create_named("Debug");
    Debug.set_function("log", Debug::log);
    Debug.set_function("dump_characters", Debug::dump_characters);
    Debug.set_function("dump_items", Debug::dump_items);
}

namespace LuaCharacter
{
void mut_damage_hp(character&, int);
void mut_apply_ailment(character&, status_ailment_t, int);
}

void LuaCharacter::mut_damage_hp(character& self, int amount)
{
    assert(amount > 0); // TODO does this need verification?
    elona::dmghp(self.idx, amount, -11); // TODO defaults to unseen hand
}

void LuaCharacter::mut_apply_ailment(character& self, status_ailment_t ailment, int power)
{
    assert(power > 0); // TODO does this need verification?
    elona::dmgcon(self.idx, ailment, power);
}

void init_usertypes(lua_env& lua)
{
    lua.get_state()->new_usertype<position_t>( "LuaPosition",
                                           sol::constructors<position_t()>(),
                                           "x", &position_t::x,
                                           "y", &position_t::y
        );
    lua.get_state()->new_usertype<character>( "LuaCharacter",
                                        "damage_hp", &LuaCharacter::mut_damage_hp,
                                        "apply_ailment", &LuaCharacter::mut_apply_ailment,
                                        "hp", sol::readonly(&character::hp),
                                        "max_hp", sol::readonly(&character::max_hp),
                                        "mp", sol::readonly(&character::mp),
                                        "max_mp", sol::readonly( &character::max_mp),
                                        "sp", sol::readonly(&character::sp),
                                        "max_sp", sol::readonly( &character::max_sp),
                                        "shop_rank", &character::shop_rank,
                                        "character_role", &character::character_role,
                                        "idx", sol::readonly(&character::idx)
        );
    lua.get_state()->new_usertype<item>( "LuaItem",
                                     "curse_state", &item::curse_state,
                                     "identify_state", &item::identification_state,
                                     "idx", sol::readonly(&item::idx)
        );
}

void init_enums(sol::table& Elona)
{
    sol::table Defines = Elona.create_named("Defines");
    sol::table Enums = Defines.create_named("Enums");

    Enums["IdentifyState"] = Enums.create_with(
        "Unidentified", identification_state_t::unidentified,
        "Partly", identification_state_t::partly_identified,
        "Almost", identification_state_t::almost_identified,
        "Completely", identification_state_t::completely_identified
        );
    Enums["CurseState"] = Enums.create_with(
        "Doomed", curse_state_t::doomed,
        "Cursed", curse_state_t::cursed,
        "None", curse_state_t::none,
        "Blessed", curse_state_t::blessed
        );
    Enums["StatusAilment"] = Enums.create_with(
        "Blinded", status_ailment_t::blinded,
        "Confused", status_ailment_t::confused,
        "Paralyzed", status_ailment_t::paralyzed,
        "Poisoned", status_ailment_t::poisoned,
        "Sleep", status_ailment_t::sleep,
        "Fear", status_ailment_t::fear,
        "Dimmed", status_ailment_t::dimmed,
        "Bleeding", status_ailment_t::bleeding,
        "Drunk", status_ailment_t::drunk,
        "Insane", status_ailment_t::insane,
        "Sick", status_ailment_t::sick
        );
    Enums["TileKind"] = Enums.create_with(
        "Normal", tile_kind_t::normal,
        "Wall", tile_kind_t::wall,
        "Tunnel", tile_kind_t::tunnel,
        "Room", tile_kind_t::room,
        "Fog", tile_kind_t::fog
        );
}

void init_api(lua_env& lua)
{
    sol::table Elona = lua.get_state()->create_named_table("Elona");

    Chara::bind(Elona);
    Pos::bind(Elona);
    World::bind(Elona);
    FOV::bind(Elona);
    Magic::bind(Elona);
    Item::bind(Elona);
    Rand::bind(Elona);
    GUI::bind(Elona);
    Map::bind(Elona);
    Debug::bind(Elona);

    init_usertypes(lua);
    init_enums(Elona);
}


} // name lua

} // namespace elona
