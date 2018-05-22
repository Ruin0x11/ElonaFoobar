#include "handle_manager.hpp"
#include "character.hpp"
#include "item.hpp"
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

    this->lua->get_state()->safe_script(R"(Handle = require "mods/core/handle")", this->handle_env);
}


void handle_manager::on_chara_creation(character& chara)
{
    if(chara_handles.find(chara.idx) != chara_handles.end())
    {
        handle_env["Handle"]["assert_chara_valid"](chara);
        return;
    }
    chara_handles.emplace(chara.idx);
    handle_env["Handle"]["create_chara_handle"](chara);
}
void handle_manager::on_item_creation(item& item)
{
    if(item_handles.find(item.idx) != item_handles.end())
    {
        handle_env["Handle"]["assert_item_valid"](item);
        return;
    }
    item_handles.emplace(item.idx);
    handle_env["Handle"]["create_item_handle"](item);
}
void handle_manager::on_chara_removal(character& chara)
{
    // TODO make into assert
    if (chara.state == 0 || chara_handles.find(chara.idx) == chara_handles.end())
    {
        handle_env["Handle"]["assert_chara_invalid"](chara);
        return;
    }
    chara_handles.erase(chara.idx);
    handle_env["Handle"]["on_chara_removal"](chara);
}
void handle_manager::on_item_removal(item& item)
{
    // item.number is set to zero often, but it doesn't seem to
    // signify whether or not the item was deleted.
    if (item_handles.find(item.idx) == item_handles.end())
    {
        handle_env["Handle"]["assert_item_invalid"](item);
        return;
    }
    item_handles.erase(item.idx);
    handle_env["Handle"]["on_item_removal"](item);
}

sol::object handle_manager::get_chara_handle(character& chara)
{
    if(chara_handles.find(chara.idx) == chara_handles.end())
    {
        std::cout << "Character " << chara.idx << " not found.";
        return sol::nil;
    }
    sol::object handle = handle_env["Handle"]["CharaHandles"][chara.idx];
    assert(handle != sol::nil);
    return handle;
}

sol::object handle_manager::get_item_handle(item& item)
{
    if(item_handles.find(item.idx) == item_handles.end())
    {
        std::cout << "Item " << item.idx << " not found.";
        return sol::nil;
    }
    sol::object handle = handle_env["Handle"]["ItemHandles"][item.idx];
    assert(handle != sol::nil);
    return handle;
}

}
}
