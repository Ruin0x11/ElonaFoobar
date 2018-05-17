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

namespace elona
{

namespace lua
{

namespace Chara {
/***
 * @submodule Chara
 */
/***
 * Checks if a character is alive.
 * @function is_alive
 * @tparam (const) LuaCharacter a character
 * @treturn bool true if the character is alive
 */
bool is_alive(const character&);

/***
 * Checks if a character is the player (has index 0).
 * @function is_player
 * @tparam LuaCharacter (const) a character
 * @treturn bool true if the character is the player
 */
bool is_player(const character&);

/***
 * Checks if a character is a member of the player's party (has index < 57).
 * @function is_ally
 * @tparam LuaCharacter (const) a character
 * @treturn bool true if the character is in the player's party
 */
bool is_ally(const character&);

/***
 * Returns a reference to the player. They might not be alive.
 * @function player
 * @treturn LuaCharacter a reference to the player
 */
sol::optional<character*> player();

/***
 * @classmod LuaCharacter
 */
/***
 * Damages a character.
 * @class character
 * @function damage_hp
 * @tparam LuaCharacter (mut) a character
 * @tparam num the amount to damage
 */
void mut_damage_hp(character&, int);

/***
 * Applies a status ailment to a character.
 * @class character
 * @function damage_con
 * @tparam LuaCharacter (mut) a character
 * @tparam Enums.StatusAilment the type of status ailment
 * @tparam num the power of the status ailment
 */
void mut_damage_con(character&, status_ailment_t, int);
};

bool Chara::is_alive(const character& chara)
{
    return chara.state == 1;
}

bool Chara::is_player(const character& chara)
{
    return chara.idx == 0;
}

bool Chara::is_ally(const character& chara)
{
    return chara.idx <= 16;
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

void Chara::mut_damage_hp(character& chara, int damage)
{
    assert(damage > 0); // TODO does this need verification?
    elona::dmghp(chara.idx, damage, -11); // TODO defaults to unseen hand
}

void Chara::mut_damage_con(character& chara, status_ailment_t type, int power)
{
    assert(power > 0); // TODO does this need verification?
    elona::dmgcon(chara.idx, type, power);
}


namespace Pos {
/***
 * @submodule Pos
 */
/***
 * Returns the distance between two points.
 * @function dist
 * @tparam LuaPosition (const) the point from
 * @tparam LuaPosition (const) the point to
 * @treturn num the distance between the points
 */
int dist(const position_t&, const position_t&);
}

int Pos::dist(const position_t& from, const position_t& to)
{
    return elona::dist(from.x, from.y, to.x, to.y);
}


namespace World {
/***
 * @submodule World
 */
/***
 * Returns the time in hours.
 * @function time
 * @treturn num the time in hours
 */
int time();
};

int World::time()
{
    return gdata_hour
        + gdata_day * 24
        + gdata_month * 24 * 30
        + gdata_year * 24 * 30 * 12;
}


namespace Magic {
/***
 * @submodule Magic
 */
/***
 * Makes the player cast a spell.
 * @function cast
 * @tparam num the spell ID
 * @tparam num the power of the spell
 * @tparam LuaPosition (const) the position the spell targets
 * @within Magic
 */
void cast(int, int, const position_t&);
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
    elona::magic();
    elona::cc = ccbk;
    elona::tc = tcbk;
}


namespace Map {
/***
 * @submodule Map
 */
/***
 * Returns the current map's width. This is only valid until the map changes.
 * @function width
 * @treturn num the current map's width in tiles
 */
int width();

/***
 * Returns the current map's height. This is only valid until the map changes.
 * @function height
 * @treturn num the current map's height in tiles
 */
int height();

/***
 * Checks if a position is inside the map.
 * @function valid
 * @tparam LuaPosition (const) the map position
 * @treturn bool true if the position is inside the map.
 */
bool valid(const position_t&);
bool valid_xy(int, int);

/***
 * Checks if a position is accessable by walking.
 * @function can_access
 * @tparam LuaPosition (const) the map position
 * @treturn bool true if the position is accessable by walking
 */
bool can_access(const position_t&);
bool can_access_xy(int, int);

/***
 * Given a position, returns a position that is bounded within the current map.
 * @function bound_within
 * @tparam LuaPosition (const) the map position
 * @treturn LuaPosition the bounded position
 */
position_t bound_within(const position_t&);

/***
 * Returns a random position in the current map. It may not be accessable.
 * @function random_pos
 * @treturn LuaPosition a random position
 */
position_t random_pos();

/***
 * Sets a tile of the current map. Only checks if the position is valid, not things like blocking objects.
 * @function set_tile
 * @tparam LuaPosition (const) the map position
 * @tparam TileKind (const) the tile kind to set
 */
void set_tile(const position_t&, tile_type_t);
void set_tile_xy(int, int, tile_type_t);

/***
 * Sets the player's memory of a tile position to the given tile kind.
 * @function set_tile_memory
 * @tparam LuaPosition (const) the map position
 * @tparam TileKind (const) the tile kind to set
 */
void set_tile_memory(const position_t&, tile_type_t);
void set_tile_memory_xy(int, int, tile_type_t);
}

int Map::width()
{
    return mdata(0);
}

int Map::height()
{
    return mdata(1);
}

bool Map::valid(const position_t& pos)
{
    return Map::valid_xy(pos.x, pos.y);
}

bool Map::valid_xy(int x, int y)
{
    if (x < 0 || y < 0 || x >= Map::width() || y >= Map::height())
    {
        return false;
    }

    return elona::map(x, y, 0) != 0;
}

bool Map::can_access(const position_t& pos)
{
    return Map::can_access_xy(pos.x, pos.y);
}

bool Map::can_access_xy(int x, int y)
{
    elona::cell_check(x, y);
    return cellaccess != 0;
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

position_t Map::random_pos()
{
    return Map::bound_within(
        position_t{
            elona::rnd(mdata(0) - 1),
                elona::rnd(mdata(1) - 1)
                });
}

void Map::set_tile(const position_t& pos, tile_type_t type)
{
    Map::set_tile_xy(pos.x, pos.y, type);
}

void Map::set_tile_xy(int x, int y, tile_type_t type)
{
    if(!Map::valid_xy(x, y))
    {
        return;
    }

    elona::map(x, y, 0) = elona::cell_get_type(type);
}

void Map::set_tile_memory(const position_t& pos, tile_type_t type)
{
    Map::set_tile_memory_xy(pos.x, pos.y, type);
}

void Map::set_tile_memory_xy(int x, int y, tile_type_t type)
{
    if(!Map::valid_xy(x, y))
    {
        return;
    }

    elona::map(x, y, 2) = elona::cell_get_type(type);
}


namespace Fov {
/***
 * @submodule Fov
 */
/***
 * Checks if there is line of sight between two positions in the current map.
 * @function los
 * @tparam LuaPosition (const) the position from
 * @tparam LuaPosition (const) the position to
 * @treturn bool true if there is line of sight between the positions
 */
bool los(const position_t&, const position_t&);

/***
 * Returns true if the player can see the given character, taking blindness into account.
 * @function you_see
 * @tparam LuaCharacter (const) a character
 * @treturn bool true if the player can see the character
 */
bool you_see(const character&);
};

bool Fov::los(const position_t& from, const position_t& to)
{
    return elona::fov_los(from.x, from.y, to.x, to.y) == 1;
}

bool Fov::you_see(const character& chara)
{
    return elona::is_in_fov(chara.idx);
}


namespace Rand {
/***
 * @submodule Rand
 */
/***
 * Returns a random number from 0 to n, exclusive.
 * @function rnd
 * @tparam num n
 * @treturn num a number in [0, n)
 */
int rnd(int n);

/***
 * Returns true one out of every n times.
 * @function one_in
 * @tparam num n
 * @treturn bool true one out of every n times
 */
bool one_in(int n);

/***
 * Returns true 50% of the time.
 * @function coinflip
 * @treturn bool true 50% of the time
 */
bool coinflip();
};


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


namespace Item {
/***
 * @submodule Item
 */
/***
 * Attempts to create an item of the given quantity at a position. Returns the item stack if it was created, nil otherwise.
 * @function create
 * @tparam LuaPosition (const) position to create the item at
 * @tparam id the item prototype id
 * @tparam num number of items to create
 * @treturn[1] LuaItem the created item stack
 * @treturn[2] nil
 */
sol::optional<item*> create(const position_t&, int, int);
sol::optional<item*> create_xy(int, int, int, int);

/***
 * Checks if an item has an enchantment.
 * @function has_enchantment
 * @tparam LuaItem (const) an item
 * @tparam num the id of the enchantment
 * @treturn bool true if the item has the enchantment
 */
bool has_enchantment(const item&, int);
}

sol::optional<item*> Item::create(const position_t& pos, int id, int num)
{
    return Item::create_xy(pos.x, pos.y, id, num);
}

sol::optional<item*> Item::create_xy(int x, int y, int id, int num)
{
    elona::flt();
    if(elona::itemcreate(-1, id, x, y, num) != 0)
    {
        return &elona::inv[ci]; // TODO deglobalize ci
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


namespace GUI {
/***
 * @submodule GUI
 */
/***
 * Prints some text in the HUD message window.
 * @function txt
 * @tparam string message to print
 * @within GUI
 */
void txt(const std::string& str);
};

void GUI::txt(const std::string& str)
{
    elona::txt(str);
}


namespace Debug
{

void dump_characters();
void dump_items();

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


void init_enums(std::unique_ptr<sol::state>& state)
{
    sol::table Elona = (*state.get())["Elona"];

    // constants
    sol::table Defines = Elona.create_named("Defines");
    sol::table Enums = Defines.create_named("Enums");

    /***
     * A series of enums.
     * @section Enum
     */
    /***
     * The identification state of an item.
     * @table IdentifyState
     */
    Enums["IdentifyState"] = Defines.create_with(
        // @type Unidentified
        "Unidentified", identification_state_t::unidentified,
        // @type Partly
        "Partly", identification_state_t::partly_identified,
        // @type Almost
        "Almost", identification_state_t::almost_identified,
        // @type Completely
        "Completely", identification_state_t::completely_identified
        );
    /***
     * The curse state of an item.
     * @table CurseState
     */
    Enums["CurseState"] = Defines.create_with(
        // @type Doomed
        "Doomed", curse_state_t::doomed,
        // @type Cursed
        "Cursed", curse_state_t::cursed,
        // @type None
        "None", curse_state_t::none,
        // @type Blessed
        "Blessed", curse_state_t::blessed
        );
    /***
     * A status effect applicable to a character.
     * @table StatusAilment
     */
    Enums["StatusAilment"] = Defines.create_with(
        // @type Blinded
        "Blinded", status_ailment_t::blinded,
        // @type Confused
        "Confused", status_ailment_t::confused,
        // @type Paralyzed
        "Paralyzed", status_ailment_t::paralyzed,
        // @type Poisoned
        "Poisoned", status_ailment_t::poisoned,
        // @type Sleep
        "Sleep", status_ailment_t::sleep,
        // @type Fear
        "Fear", status_ailment_t::fear,
        // @type Dimmed
        "Dimmed", status_ailment_t::dimmed,
        // @type Bleeding
        "Bleeding", status_ailment_t::bleeding,
        // @type Drunk
        "Drunk", status_ailment_t::drunk,
        // @type Insane
        "Insane", status_ailment_t::insane,
        // @type Sick
        "Sick", status_ailment_t::sick
        );
    /***
     * A type of tile to generate with Map.set_tile.
     * @table TileType
     * @see Map.set_tile
     */
    Enums["TileType"] = Defines.create_with(
        // @type Normal
        "Normal", tile_type_t::normal,
        // @type Wall
        "Wall", tile_type_t::wall,
        // @type Tunnel
        "Tunnel", tile_type_t::tunnel,
        // @type Room
        "Room", tile_type_t::room,
        // @type Fog
        "Fog", tile_type_t::fog
        );
}

void init_api(std::unique_ptr<sol::state>& state)
{
    state.get()->new_usertype<position_t>( "position",
                                         sol::constructors<position_t()>()
        );
    state.get()->new_usertype<character>( "character",
                                        "damage_hp", &Chara::mut_damage_hp,
                                        "damage_con", &Chara::mut_damage_con,
                                        "idx", sol::readonly( &character::idx )
        );
    state.get()->new_usertype<item>( "item",
                                     "curse_state", &item::curse_state,
                                     "identify_state", &item::identification_state,
                                     "idx", sol::readonly( &item::idx )
        );

    sol::table Elona = state.get()->create_named_table("Elona");
    Elona.set_function("log", [](const std::string& msg) { elona::log::detail::out << msg << std::endl; } );

    sol::table Chara = Elona.create_named("Chara");
    Chara.set_function("is_alive", Chara::is_alive);
    Chara.set_function("is_player", Chara::is_player);
    Chara.set_function("is_ally", Chara::is_ally);
    Chara.set_function("player", Chara::player);

    sol::table Fov = Elona.create_named("Fov");
    Fov.set_function("los", Fov::los);
    Fov.set_function("you_see", Fov::you_see);

    sol::table Item = Elona.create_named("Item");
    Item.set_function("create", sol::overload(Item::create, Item::create_xy));

    sol::table Rand = Elona.create_named("Rand");
    Rand.set_function("rnd", Rand::rnd);
    Rand.set_function("one_in", Rand::one_in);
    Rand.set_function("coinflip", Rand::coinflip);

    sol::table Magic = Elona.create_named("Magic");
    Magic.set_function("cast", Magic::cast);

    sol::table Map = Elona.create_named("Map");
    Map.set_function("valid", Map::valid);
    Map.set_function("can_access", sol::overload(Map::can_access, Map::can_access_xy));
    Map.set_function("bound_within", Map::bound_within);
    Map.set_function("random_pos", Map::random_pos);
    Map.set_function("set_tile", sol::overload(Map::set_tile, Map::set_tile_xy));
    Map.set_function("set_tile_memory", sol::overload(Map::set_tile_memory, Map::set_tile_memory_xy));

    sol::table GUI = Elona.create_named("GUI");
    GUI.set_function("txt", GUI::txt);

    sol::table Debug = Elona.create_named("Debug");
    Debug.set_function("dump_characters", Debug::dump_characters);
    Debug.set_function("dump_items", Debug::dump_items);

    init_enums(state);
}


} // name lua

} // namespace elona