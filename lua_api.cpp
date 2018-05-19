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
 * @module Elona
 */
namespace elona
{

namespace lua
{

namespace Chara {
/***
 * Checks if a character is alive.
 * @function is_alive
 * @param character (const) LuaCharacter a character
 * @treturn bool true if the character is alive
 * @within Elona.Chara
 */
bool is_alive(const character&);

/***
 * Checks if a character is the player (has index 0).
 * @function is_player
 * @param character (const) a character
 * @treturn bool true if the character is the player
 * @within Elona.Chara
 */
bool is_player(const character&);

/***
 * Checks if a character is a member of the player's party (has index < 16).
 * @function is_ally
 * @param character (const) a character
 * @treturn bool true if the character is in the player's party
 * @within Elona.Chara
 */
bool is_ally(const character&);

/***
 * Returns a reference to the player. They might not be alive.
 * @function player
 * @treturn LuaCharacter a reference to the player
 * @within Elona.Chara
 */
sol::optional<character*> player();

/***
 * Attempts to create a character at a given position. Returns the character if it was created, nil otherwise.
 * @function create
 * @param character (const) position to create the character at
 * @param id the character prototype id
 * @treturn[1] LuaCharacter the created character
 * @treturn[2] nil
 * @within Elona.Chara
 */
sol::optional<character*> create(const position_t&, int);
sol::optional<character*> create_xy(int, int, int);


/***
 * Damages a character.
 * @function damage_hp
 * @param amount the amount to damage
 * @within LuaCharacter
 */
void mut_damage_hp(character&, int);

/***
 * Applies a status ailment to a character.
 * @function apply_ailment
 * @param type the type of status ailment
 * @param power the power of the status ailment
 * @within LuaCharacter
 */
void mut_apply_ailment(character&, status_ailment_t, int);
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
    return character.idx <= 16;
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


void Chara::mut_damage_hp(character& self, int amount)
{
    assert(amount > 0); // TODO does this need verification?
    elona::dmghp(self.idx, amount, -11); // TODO defaults to unseen hand
}

void Chara::mut_apply_ailment(character& self, status_ailment_t type, int power)
{
    assert(power > 0); // TODO does this need verification?
    elona::dmgcon(self.idx, type, power);
}


namespace Pos {
/***
 * Returns the distance between two points.
 * @function dist
 * @param from (const) the point from
 * @param to (const) the point to
 * @treturn num the distance between the points
 * @within Elona.Pos
 */
int dist(const position_t&, const position_t&);
}

int Pos::dist(const position_t& from, const position_t& to)
{
    return elona::dist(from.x, from.y, to.x, to.y);
}


namespace World {
/***
 * Returns the time in hours.
 * @function time
 * @treturn num the time in hours
 * @within Elona.World
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
 * Makes the player cast a spell.
 * @function cast
 * @param effect_id the spell ID
 * @param effect_power the power of the spell
 * @param target_location (const) the position the spell targets
 * @within Elona.Magic
 */
void cast(int, int, const position_t&);
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


namespace Map {
/***
 * Returns the current map's width. This is only valid until the map changes.
 * @function width
 * @treturn num the current map's width in tiles
 * @within Elona.Map
 */
int width();

/***
 * Returns the current map's height. This is only valid until the map changes.
 * @function height
 * @treturn num the current map's height in tiles
 * @within Elona.Map
 */
int height();

/***
 * Checks if a position is inside the map.
 * @function valid
 * @param position (const) the map position
 * @treturn bool true if the position is inside the map.
 * @within Elona.Map
 */
bool valid(const position_t&);
bool valid_xy(int, int);

/***
 * Checks if a position is accessable by walking.
 * @function can_access
 * @param position (const) the map position
 * @treturn bool true if the position is accessable by walking
 * @within Elona.Map
 */
bool can_access(const position_t&);
bool can_access_xy(int, int);

/***
 * Given a position, returns a position that is bounded within the current map.
 * @function bound_within
 * @param position (const) the map position
 * @treturn LuaPosition the bounded position
 * @within Elona.Map
 */
position_t bound_within(const position_t&);

/***
 * Returns a random position in the current map. It may not be accessable.
 * @function random_pos
 * @treturn LuaPosition a random position
 * @within Elona.Map
 */
position_t random_pos();

/***
 * Sets a tile of the current map. Only checks if the position is valid, not things like blocking objects.
 * @function set_tile
 * @param position (const) the map position
 * @param type (const) the type type to set
 * @within Elona.Map
 */
void set_tile(const position_t&, tile_type_t);
void set_tile_xy(int, int, tile_type_t);

/***
 * Sets the player's memory of a tile position to the given tile kind.
 * @function set_tile_memory
 * @param position (const) the map position
 * @param type (const) the tile type to set
 * @within Elona.Map
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

void Map::set_tile(const position_t& position, tile_type_t type)
{
    Map::set_tile_xy(position.x, position.y, type);
}

void Map::set_tile_xy(int x, int y, tile_type_t type)
{
    if(!Map::valid_xy(x, y))
    {
        return;
    }

    elona::map(x, y, 0) = elona::cell_get_type(type);
}

void Map::set_tile_memory(const position_t& position, tile_type_t type)
{
    Map::set_tile_memory_xy(position.x, position.y, type);
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
 * Checks if there is line of sight between two positions in the current map.
 * @function los
 * @param from (const) the position from
 * @param to (const) the position to
 * @treturn bool true if there is line of sight between the positions
 * @within Elona.Fov
 */
bool los(const position_t&, const position_t&);

/***
 * Returns true if the player can see the given character, taking blindness into account.
 * @function you_see
 * @param character (const) a character
 * @treturn bool true if the player can see the character
 * @within Elona.Fov
 */
bool you_see(const character&);
};

bool Fov::los(const position_t& from, const position_t& to)
{
    return elona::fov_los(from.x, from.y, to.x, to.y) == 1;
}

bool Fov::you_see(const character& character)
{
    return elona::is_in_fov(character.idx);
}


namespace Rand {
/***
 * Returns a random number from 0 to n, exclusive.
 * @function rnd
 * @param n
 * @treturn num a number in [0, n)
 * @within Elona.Rand
 */
int rnd(int n);

/***
 * Returns true one out of every n times.
 * @function one_in
 * @param n
 * @treturn bool true one out of every n times
 * @within Elona.Rand
 */
bool one_in(int n);

/***
 * Returns true 50% of the time.
 * @function coinflip
 * @treturn bool true 50% of the time
 * @within Elona.Rand
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
 * Attempts to create an item of the given quantity at a position. Returns the item stack if it was created, nil otherwise.
 * @function create
 * @param position (const) position to create the item at
 * @param id the item prototype id
 * @param number the number of items to create
 * @treturn[1] LuaItem the created item stack
 * @treturn[2] nil
 * @within Elona.Item
 */
sol::optional<item*> create(const position_t&, int, int);
sol::optional<item*> create_xy(int, int, int, int);

/***
 * Checks if an item has an enchantment.
 * @param item (const) an item
 * @param id the id of the enchantment
 * @treturn bool true if the item has the enchantment
 */
bool has_enchantment(const item&, int);
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


namespace GUI {
/***
 * Prints some text in the HUD message window.
 * @function txt
 * @param str message to print
 * @within Elona.GUI
 */
void txt(const std::string& str);
};

void GUI::txt(const std::string& str)
{
    elona::txt(str);
}


namespace Debug
{

/***
 * Logs a message to log.txt.
 * @function log
 * @param message the message to log
 * @within Elona.Debug
 */
void log(const std::string&);

/***
 * Dumps all characters to the log.
 * @function dump_characters
 * @within Elona.Debug
 */
void dump_characters();

/***
 * Dumps all items to the log.
 * @function dump_items
 * @within Elona.Debug
 */
void dump_items();

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


void init_enums(std::unique_ptr<sol::state>& state)
{
    sol::table Elona = (*state.get())["Elona"];

    // constants
    sol::table Defines = Elona.create_named("Defines");
    sol::table Enums = Defines.create_named("Enums");

    /***
     * Enums
     * @section Enums
     */
    /***
     * The identification state of an item.
     * @type Elona.Enums.IdentifyState
     */
    Enums["IdentifyState"] = Defines.create_with(
        /***
         * @field Unidentified
         */
        "Unidentified", identification_state_t::unidentified,
        /***
         * @field Partly
         */
        "Partly", identification_state_t::partly_identified,
        /***
         * @field Almost
         */
        "Almost", identification_state_t::almost_identified,
        /***
         * @field Completely
         */
        "Completely", identification_state_t::completely_identified
        );
    /***
     * The curse state of an item.
     * @type Elona.Enums.CurseState
     */
    Enums["CurseState"] = Defines.create_with(
        /***
         * @field Doomed
         */
        "Doomed", curse_state_t::doomed,
        /***
         * @field Cursed
         */
        "Cursed", curse_state_t::cursed,
        /***
         * @field None
         */
        "None", curse_state_t::none,
        /***
         * @field Blessed
         */
        "Blessed", curse_state_t::blessed
        );
    /***
     * A status effect applicable to a character.
     * @type Elona.Enums.StatusAilment
     */
    Enums["StatusAilment"] = Defines.create_with(
        /***
         * @field Blinded
         */
        "Blinded", status_ailment_t::blinded,
        /***
         * @field Confused
         */
        "Confused", status_ailment_t::confused,
        /***
         * @field Paralyzed
         */
        "Paralyzed", status_ailment_t::paralyzed,
        /***
         * @field Poisoned
         */
        "Poisoned", status_ailment_t::poisoned,
        /***
         * @field Sleep
         */
        "Sleep", status_ailment_t::sleep,
        /***
         * @field Fear
         */
        "Fear", status_ailment_t::fear,
        /***
         * @field Dimmed
         */
        "Dimmed", status_ailment_t::dimmed,
        /***
         * @field Bleeding
         */
        "Bleeding", status_ailment_t::bleeding,
        /***
         * @field Drunk
         */
        "Drunk", status_ailment_t::drunk,
        /***
         * @field Insane
         */
        "Insane", status_ailment_t::insane,
        /***
         * @field Sick
         */
        "Sick", status_ailment_t::sick
        );
    /***
     * A type of tile to generate with Map.set_tile.
     * @type Elona.Enums.TileType
     */
    Enums["TileType"] = Defines.create_with(
        /***
         * @field Normal
         */
        "Normal", tile_type_t::normal,
        /***
         * @field Wall
         */
        "Wall", tile_type_t::wall,
        /***
         * @field Tunnel
         */
        "Tunnel", tile_type_t::tunnel,
        /***
         * @field Room
         */
        "Room", tile_type_t::room,
        /***
         * @field Fog
         */
        "Fog", tile_type_t::fog
        );
}

void init_api(std::unique_ptr<sol::state>& state)
{
    state.get()->new_usertype<position_t>( "LuaPosition",
                                         sol::constructors<position_t()>()
        );
    state.get()->new_usertype<character>( "LuaCharacter",
                                        "damage_hp", &Chara::mut_damage_hp,
                                        "apply_ailment", &Chara::mut_apply_ailment,
                                        "idx", sol::readonly(&character::idx),
                                        "hp", sol::readonly(&character::hp),
                                        "max_hp", sol::readonly(&character::max_hp),
                                        "mp", sol::readonly(&character::mp),
                                        "max_mp", sol::readonly( &character::max_mp),
                                        "sp", sol::readonly(&character::sp),
                                        "max_sp", sol::readonly( &character::max_sp),
                                        "shop_rank", &character::shop_rank,
                                        "character_role", &character::character_role
        );
    state.get()->new_usertype<item>( "LuaItem",
                                     "curse_state", &item::curse_state,
                                     "identify_state", &item::identification_state,
                                     "idx", sol::readonly(&item::idx)
        );

    sol::table Elona = state.get()->create_named_table("Elona");
    Elona.set_function("log", Debug::log);

    sol::table Chara = Elona.create_named("Chara");
    Chara.set_function("is_alive", Chara::is_alive);
    Chara.set_function("is_player", Chara::is_player);
    Chara.set_function("is_ally", Chara::is_ally);
    Chara.set_function("player", Chara::player);
    Chara.set_function("create", sol::overload(Chara::create, Chara::create_xy));

    sol::table Pos = Elona.create_named("Pos");
    Pos.set_function("dist", Pos::dist);

    sol::table World = Elona.create_named("World");
    World.set_function("time", Pos::time);

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
    Map.set_function("width", Map::width);
    Map.set_function("height", Map::height);
    Map.set_function("valid", sol::overload(Map::valid, Map::valid_xy));
    Map.set_function("can_access", sol::overload(Map::can_access, Map::can_access_xy));
    Map.set_function("bound_within", Map::bound_within);
    Map.set_function("random_pos", Map::random_pos);
    Map.set_function("set_tile", sol::overload(Map::set_tile, Map::set_tile_xy));
    Map.set_function("set_tile_memory", sol::overload(Map::set_tile_memory, Map::set_tile_memory_xy));

    sol::table GUI = Elona.create_named("GUI");
    GUI.set_function("txt", GUI::txt);

    sol::table Debug = Elona.create_named("Debug");
    Debug.set_function("log", Debug::log);
    Debug.set_function("dump_characters", Debug::dump_characters);
    Debug.set_function("dump_items", Debug::dump_items);

    init_enums(state);
}


} // name lua

} // namespace elona
