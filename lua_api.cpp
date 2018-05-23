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
#include "ui.hpp"
#include "variables.hpp"
#include <iterator>

/***
 * See doc/api for the API documentation.
 * Be sure to update it if you change anything here.
 * LDoc is completely broken when it comes to parsing C++.
 */
namespace elona
{

namespace lua
{

typedef sol::table lua_character_handle;
typedef sol::table lua_item_handle;

// Marshal Lua handles to a C++ references.

// This is needed so if a C++ reference goes invalid, the Lua side
// will know and be able to raise an error. But that means that the
// handles (which are Lua tables) will be passed as arguments to the
// API instead of the underlying usertype.

// Note that these don't apply to methods called on the handles,
// because the userdata metatable will be able to pass those functions
// the raw C++ reference from inside the __index metamethod. It's just
// that if a user passes a handle as an argument to an API function
// implemented in C++, it's not possible to transparently get the
// reference out of the handle before the function is called without
// having to marshal it by the time it's reached the C++ side.

// NOTE: a side effect of this is that overloaded methods that can
// take a handle argument must be ordered last inside sol::overload(),
// because if an argument can be converted to a Lua table the handler
// for the Lua table will be called first (since handles are Lua
// tables), which is wrong. It also means that overloaded methods
// cannot take either a character or item.

character& conv_chara(lua_character_handle handle)
{
    sol::object obj = handle["cpp_ref"];
    bool is_valid = handle["is_valid"];
    if(!is_valid || !obj.is<character&>())
    {
        std::cerr << "Handle not valid" << std::endl;
        throw new sol::error("not valid handle");
    }
    return obj.as<character&>();
}

item& conv_item(lua_item_handle handle)
{
    sol::object obj = handle["cpp_ref"];
    bool is_valid = handle["is_valid"];
    if(!is_valid || !obj.is<item&>())
    {
        std::cerr << "Handle not valid" << std::endl;
        throw new sol::error("not valid handle");
    }
    return obj.as<item&>();
}

namespace Chara {
bool is_alive(const lua_character_handle);
bool is_player(const lua_character_handle);
bool is_ally(const lua_character_handle);
bool add_ally(lua_character_handle);
sol::optional<lua_character_handle> player();
sol::optional<lua_character_handle> create(const position_t&, int);
sol::optional<lua_character_handle> create_xy(int, int, int);

void bind(sol::table& Elona);
};

bool Chara::is_alive(const lua_character_handle handle)
{
    return conv_chara(handle).state == 1;
}

bool Chara::is_player(const lua_character_handle handle)
{
    return conv_chara(handle).idx == 0;
}

bool Chara::is_ally(const lua_character_handle handle)
{
    return !Chara::is_player(handle) && conv_chara(handle).idx <= 16;
}

sol::optional<lua_character_handle> Chara::player()
{
    if(elona::cdata[0].state == 0) {
        return sol::nullopt;
    }
    else
    {
        lua_character_handle handle = lua::lua.get_handle_manager().get_chara_handle(elona::cdata[0]);
        return handle;
    }
}

sol::optional<lua_character_handle> Chara::create(const position_t& position, int id)
{
    return Chara::create_xy(position.x, position.y, id);
}

sol::optional<lua_character_handle> Chara::create_xy(int x, int y, int id)
{
    elona::flt();
    if(elona::chara_create(-1, id, x, y) != 0)
    {
        lua_character_handle handle = lua::lua.get_handle_manager().get_chara_handle(elona::cdata[elona::rc]);
        return handle;
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
    if(World::is_in_overworld())
    {
        return false;
    }
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
    if(World::is_in_overworld())
    {
        return false;
    }
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
    if(World::is_in_overworld())
    {
        return;
    }
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
    if(World::is_in_overworld())
    {
        return;
    }
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
bool los_xy(int, int, int, int);
bool you_see(const lua_character_handle);
bool you_see_pos(const position_t&);
bool you_see_pos_xy(int, int);
void refresh();

void bind(sol::table& Elona);
};

bool FOV::los(const position_t& from, const position_t& to)
{
    return FOV::los_xy(from.x, from.y, to.x, to.y);
}

bool FOV::los_xy(int fx, int fy, int tx, int ty)
{
    return elona::fov_los(fx, fy, tx, ty) == 1;
}

bool FOV::you_see(const lua_character_handle handle)
{
    return elona::is_in_fov(conv_chara(handle).idx);
}

bool FOV::you_see_pos(const position_t& pos)
{
    return elona::is_in_fov(pos) == 1;
}

bool FOV::you_see_pos_xy(int x, int y)
{
    return elona::is_in_fov(position_t(x, y)) == 1;
}

void FOV::refresh()
{
    gmode(2);
    sxfix = 0;
    syfix = 0;
    update_scrolling_info();
    update_slight();
    label_1433();
}

void FOV::bind(sol::table& Elona)
{
    sol::table FOV = Elona.create_named("FOV");
    FOV.set_function("los", sol::overload(FOV::los, FOV::los_xy));
    FOV.set_function("you_see", sol::overload(FOV::you_see_pos,
                                              FOV::you_see_pos_xy,
                                              FOV::you_see));
    FOV.set_function("refresh", FOV::refresh);
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
sol::optional<lua_item_handle> create(const position_t&, int, int);
sol::optional<lua_item_handle> create_xy(int, int, int, int);
bool has_enchantment(const lua_item_handle, int);

void bind(sol::table& Elona);
}

sol::optional<lua_item_handle> Item::create(const position_t& position, int id, int number)
{
    return Item::create_xy(position.x, position.y, id, number);
}

sol::optional<lua_item_handle> Item::create_xy(int x, int y, int id, int number)
{
    elona::flt();
    if(elona::itemcreate(-1, id, x, y, number) != 0)
    {
        lua_item_handle handle = lua::lua.get_handle_manager().get_item_handle(elona::inv[elona::ci]); // TODO deglobalize ci
        return handle;
    }
    else
    {
        return sol::nullopt;
    }
}

bool Item::has_enchantment(const lua_item_handle handle, int id)
{
    return elona::encfindspec(conv_item(handle).idx, id);
}

void Item::bind(sol::table& Elona)
{
    sol::table Item = Elona.create_named("Item");
    Item.set_function("create", sol::overload(Item::create, Item::create_xy));
    Item.set_function("has_enchantment", Item::has_enchantment);
}


namespace GUI {
void txt(const std::string&);
void txt_color(int);

void bind(sol::table&);
};

void GUI::txt(const std::string& message)
{
    elona::txt(message);
}

void GUI::txt_color(int color)
{
    if(color < 0 || color > 20)
    {
        return;
    }
    elona::txtef(color);
}

void GUI::bind(sol::table& Elona)
{
    sol::table GUI = Elona.create_named("GUI");
    GUI.set_function("txt", GUI::txt);
    GUI.set_function("txt_color", GUI::txt_color);
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
void damage_hp(character&, int);
void apply_ailment(character&, status_ailment_t, int);
bool recruit_as_ally(character&);
}

void LuaCharacter::damage_hp(character& self, int amount)
{
    assert(amount > 0); // TODO does this need verification?
    elona::dmghp(self.idx, amount, -11); // TODO defaults to unseen hand
}

void LuaCharacter::apply_ailment(character& self, status_ailment_t ailment, int power)
{
    assert(power > 0); // TODO does this need verification?
    elona::dmgcon(self.idx, ailment, power);
}

bool LuaCharacter::recruit_as_ally(character& self)
{
    // can't use Chara methods because they take a handle...
    if(self.state == 0 || (self.idx != 0 && self.idx <= 16) || self.idx == 0)
    {
        return false;
    }
    elona::rc = self.idx;
    return new_ally_joins() == 1;
}

void init_usertypes(lua_env& lua)
{
    lua.get_state()->new_usertype<position_t>( "LuaPosition",
                                            sol::constructors<position_t(), position_t(int, int)>(),
                                           "x", &position_t::x,
                                           "y", &position_t::y
        );
    lua.get_state()->new_usertype<character>( "LuaCharacter",
                                        "damage_hp", &LuaCharacter::damage_hp,
                                        "apply_ailment", &LuaCharacter::apply_ailment,
                                        "recruit_as_ally", &LuaCharacter::recruit_as_ally,
                                        "hp", sol::readonly(&character::hp),
                                        "max_hp", sol::readonly(&character::max_hp),
                                        "mp", sol::readonly(&character::mp),
                                        "max_mp", sol::readonly(&character::max_mp),
                                        "sp", sol::readonly(&character::sp),
                                        "max_sp", sol::readonly(&character::max_sp),
                                        "shop_rank", &character::shop_rank,
                                        "character_role", &character::character_role,
                                        "idx", sol::readonly(&character::idx),
                                        "position", &character::position
        );
    lua.get_state()->new_usertype<item>( "LuaItem",
                                     "curse_state", &item::curse_state,
                                     "identify_state", &item::identification_state,
                                     "idx", sol::readonly(&item::idx),
                                     "position", &item::position,
                                     "number", &item::number
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

api_manager::api_manager(lua_env* lua)
{
    this->lua = lua;
    this->api_env = sol::environment(*(this->lua->get_state()),
                                     sol::create,
                                     this->lua->get_state()->globals());

    // Bind the API tables at e.g. Elona["core"]["Chara"]
    sol::table Elona = api_env.create_named("Elona");
    sol::table core = Elona.create_named("core");

    Chara::bind(core);
    Pos::bind(core);
    World::bind(core);
    FOV::bind(core);
    Magic::bind(core);
    Item::bind(core);
    Rand::bind(core);
    GUI::bind(core);
    Map::bind(core);
    Debug::bind(core);

    init_enums(core);

    // register usertypes globally, so the handle manager can get at them.
    init_usertypes(*lua);
}

sol::optional<sol::table> api_manager::try_find_api(const std::string& parent, const std::string& module)
{
    sol::optional<sol::table> table = api_env["Elona"][parent];
    if (!table)
    {
        return sol::nullopt;
    }
    sol::optional<sol::table> result = (*table)[module];
    return result;
}

void api_manager::load_core(lua_env& lua, const fs::path& mods_dir)
{
    auto result = lua.get_state()->safe_script_file(filesystem::make_preferred_path_in_utf8(
                                           mods_dir / "core" / "init.lua"),
                                       api_env);
    if (!result.valid())
    {
        sol::error err = result;
        std::string what = err.what();
        ELONA_LOG(what);
        throw new std::runtime_error("Failed initializing core mod!");
    }
}

int deny_function(lua_State* L)
{
    return luaL_error(L, "This function cannot be called.");
}

void api_manager::bind(lua_env& lua, mod_info& mod)
{
    mod.env.create_named("Elona",
                         "require", sol::overload(

                             [&lua](const std::string& parent, const std::string& module) {
                                 sol::optional<sol::table> result = sol::nullopt;
                                 result = lua.get_api_manager().try_find_api(parent, module);
                                 return result;
                             },

                             // If no mod name is provided, assume it is "core".
                             [&lua](const std::string& module) {
                                 sol::optional<sol::table> result = sol::nullopt;
                                 result = lua.get_api_manager().try_find_api("core", module);
                                 return result;
                             }
                             ));
}


sol::table api_manager::get_api_table()
{
    return api_env["Elona"]["core"];
}


// for testing usage
void api_manager::bind(lua_env& lua)
{
    lua.get_state()->create_named_table("Elona",
                         "require", sol::overload(

                             [&lua](const std::string& parent, const std::string& module) {
                                 sol::optional<sol::table> result = sol::nullopt;
                                 result = lua.get_api_manager().try_find_api(parent, module);
                                 return result;
                             },

                             // If no mod name is provided, assume it is "core".
                             [&lua](const std::string& module) {
                                 sol::optional<sol::table> result = sol::nullopt;
                                 result = lua.get_api_manager().try_find_api("core", module);
                                 return result;
                             }
                             ));
}

} // name lua

} // namespace elona
