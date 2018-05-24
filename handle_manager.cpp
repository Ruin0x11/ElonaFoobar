#include "handle_manager.hpp"
#include "character.hpp"
#include "config.hpp"
#include "item.hpp"
#include "log.hpp"
#include "lua.hpp"
#include <cassert>
#include <set>

namespace elona
{
namespace lua
{

handle_manager::handle_manager(lua_env* lua)
{
    this->lua = lua;
    this->handle_env = sol::environment(*(this->lua->get_state()),
                                        sol::create,
                                        this->lua->get_state()->globals());

    this->lua->get_state()->set("is_test", config::instance().is_test);
    this->lua->get_state()->safe_script(R"(Handle = require "mods/core/handle")", this->handle_env);
}

void handle_manager::create_chara_handle(character& chara)
{
    if(chara_handles.find(chara.idx) != chara_handles.end())
    {
        handle_env["Handle"]["assert_chara_valid"](chara);
        return;
    }
    chara_handles.emplace(chara.idx);
    handle_env["Handle"]["create_chara_handle"](chara);
}

void handle_manager::create_item_handle(item& item)
{
    if(item_handles.find(item.idx) != item_handles.end())
    {
        handle_env["Handle"]["assert_item_valid"](item);
        return;
    }
    item_handles.emplace(item.idx);
    handle_env["Handle"]["create_item_handle"](item);
}

void handle_manager::remove_chara_handle(character& chara)
{
    // TODO should chara.state == 0 mean the handle is invalid?
    // Some characters can die and respawn again.
    if (chara_handles.find(chara.idx) == chara_handles.end())
    {
        handle_env["Handle"]["assert_chara_invalid"](chara);
        return;
    }
    chara_handles.erase(chara.idx);
    handle_env["Handle"]["remove_chara_handle"](chara);
}

void handle_manager::remove_item_handle(item& item)
{
    // item.number is set to zero often, but it doesn't seem to
    // signify whether or not the item was deleted.
    if (item_handles.find(item.idx) == item_handles.end())
    {
        handle_env["Handle"]["assert_item_invalid"](item);
        return;
    }
    item_handles.erase(item.idx);
    handle_env["Handle"]["remove_item_handle"](item);
}

sol::object handle_manager::get_chara_handle(character& chara)
{
    if(chara.idx == -1)
    {
        ELONA_LOG("Tried getting handle to character of index -1");
        return sol::nil;
    }
    if(chara_handles.find(chara.idx) == chara_handles.end())
    {
        //std::cout << "Character " << chara.idx << " not found." << std::endl;
        return sol::nil;
    }
    sol::object handle = handle_env["Handle"]["CharaHandles"][chara.idx];
    assert(handle != sol::nil);
    return handle;
}

sol::object handle_manager::get_item_handle(item& item)
{
    if(item.idx == -1)
    {
        ELONA_LOG("Tried getting handle to item of index -1");
        return sol::nil;
    }
    if(item_handles.find(item.idx) == item_handles.end())
    {
        //std::cout << "Item " << item.idx << " not found." << std::endl;
        return sol::nil;
    }
    sol::object handle = handle_env["Handle"]["ItemHandles"][item.idx];
    assert(handle != sol::nil);
    return handle;
}

void handle_manager::clear()
{
    chara_handles.clear();
    item_handles.clear();
    handle_env["Handle"]["CharaHandles"] = this->lua->get_state()->create_table_with();
    handle_env["Handle"]["ItemHandles"] = this->lua->get_state()->create_table_with();
}

}
}
