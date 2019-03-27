#include "lua_class_game_data.hpp"

namespace elona
{
namespace lua
{

#define ELONA_LUA_PROPERTY(field) \
    sol::property( \
        [](GameData& it) { return it.field; }, \
        [](GameData& it, int value) { it.field = value; })

#define ELONA_LUA_FLAG(field) \
    sol::property( \
        [](GameData& it) { return it.field == 1; }, \
        [](GameData& it, bool value) { it.field = value ? 1 : 0; })


void LuaGameData::bind(sol::state& lua)
{
    auto LuaGameData = lua.create_simple_usertype<GameData>();
    LuaGameData.set("new", sol::no_constructor);

    /**
     * @luadoc home_scale field num
     *
     * [RW] The player's home scale.
     */
    LuaGameData.set("home_scale", &GameData::home_scale);

    /**
     * @luadoc initial_cart_limit field num
     *
     * [RW] The player's initial cargo capacity.
     */
    LuaGameData.set("initial_cart_limit", &GameData::initial_cart_limit);

    /**
     * @luadoc current_cart_limit field num
     *
     * [RW] The player's maximum cargo capacity.
     */
    LuaGameData.set("current_cart_limit", &GameData::current_cart_limit);

    /**
     * @luadoc ranks field table
     */
    LuaGameData.set("ranks", &GameData::ranks);

    /**
     * @luadoc left_bill field num
     *
     * [RW] The number of unpaid bills the player owns.
     */
    LuaGameData.set("left_bill", &GameData::left_bill);

    /**
     * @luadoc date field LuaDateTime
     *
     * [R] The current date and time.
     */
    LuaGameData.set("date", &GameData::date);


    /**
     * @luadoc belongs_to_mages_guild field num
     */
    LuaGameData.set(
        "belongs_to_mages_guild",
        ELONA_LUA_PROPERTY(guild.belongs_to_mages_guild));

    /**
     * @luadoc belongs_to_fighters_guild field num
     */
    LuaGameData.set(
        "belongs_to_fighters_guild",
        ELONA_LUA_PROPERTY(guild.belongs_to_fighters_guild));

    /**
     * @luadoc belongs_to_thieves_guild field num
     */
    LuaGameData.set(
        "belongs_to_thieves_guild",
        ELONA_LUA_PROPERTY(guild.belongs_to_thieves_guild));

    /**
     * @luadoc mages_guild_quota field num
     */
    LuaGameData.set(
        "mages_guild_quota", ELONA_LUA_PROPERTY(guild.mages_guild_quota));

    /**
     * @luadoc fighters_guild_quota field num
     */
    LuaGameData.set(
        "fighters_guild_quota", ELONA_LUA_PROPERTY(guild.fighters_guild_quota));

    /**
     * @luadoc fighters_guild_target field num
     */
    LuaGameData.set(
        "fighters_guild_target",
        ELONA_LUA_PROPERTY(guild.fighters_guild_target));

    /**
     * @luadoc thieves_guild_quota field num
     */
    LuaGameData.set(
        "thieves_guild_quota", ELONA_LUA_PROPERTY(guild.thieves_guild_quota));

    /**
     * @luadoc joining_mages_guild field num
     */
    LuaGameData.set(
        "joining_mages_guild", ELONA_LUA_PROPERTY(guild.joining_mages_guild));

    /**
     * @luadoc joining_fighters_guild field num
     */
    LuaGameData.set(
        "joining_fighters_guild",
        ELONA_LUA_PROPERTY(guild.joining_fighters_guild));

    /**
     * @luadoc joining_thieves_guild field num
     */
    LuaGameData.set(
        "joining_thieves_guild",
        ELONA_LUA_PROPERTY(guild.joining_thieves_guild));

    /**
     * @luadoc mages_guild_quota_recurring field bool
     */
    LuaGameData.set(
        "mages_guild_quota_recurring",
        ELONA_LUA_FLAG(guild.mages_guild_quota_recurring));

    /**
     * @luadoc fighters_guild_quota_recurring field bool
     */
    LuaGameData.set(
        "fighters_guild_quota_recurring",
        ELONA_LUA_FLAG(guild.fighters_guild_quota_recurring));

    /**
     * @luadoc thieves_guild_quota_recurring field bool
     */
    LuaGameData.set(
        "thieves_guild_quota_recurring",
        ELONA_LUA_FLAG(guild.thieves_guild_quota_recurring));

    /**
     * @luadoc wizard field bool
     *
     * [RW] The wizard status of the game.
     */
    LuaGameData.set("wizard", ELONA_LUA_FLAG(wizard));


    lua.set_usertype("LuaGameData", LuaGameData);
}

} // namespace lua
} // namespace elona
