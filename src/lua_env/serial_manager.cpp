#include "serial_manager.hpp"
#include "lua_env.hpp"
#include "../putit.hpp"

namespace elona
{
namespace lua
{

serial_manager::serial_manager(lua_env* _lua)
{
    lua = _lua;
    serial_env = sol::environment(*(lua->get_state()), sol::create);

    // Allow the resolution of handles when loading.
    serial_env["Handle"] = lua->get_handle_manager().get_handle_env();

    // Load the Lua chunk for saving/loading data.
    lua->get_state()->safe_script(
        R"(Serial = require "mods/core/serial")", serial_env);
}

void serial_manager::save_all_data(sol::environment& mod_env)
{

}

void serial_manager::load_all_data(sol::environment& mod_env)
{

}

void serial_manager::save_map_local_data(sol::environment& mod_env)
{

}

void serial_manager::load_map_local_data(sol::environment& mod_env)
{

}

void serial_manager::save(sol::table& data, fs::path output_path)
{
    std::ofstream out{output_path.native(), std::ios::binary};
    if (out.fail())
    {
        throw std::runtime_error(
            u8"Error: fail to write "
            + filesystem::make_preferred_path_in_utf8(output_path));
    }
    putit::binary_oarchive ar{out};

    serial_env["_TO_SERIALIZE"] = data;
    auto result = lua->get_state()->safe_script(R"(return Serial.save(_TO_SERIALIZE))", serial_env);
    serial_env["_TO_SERIALIZE"] = sol::lua_nil;

    if (result.valid())
    {
        std::string dump = result.get<std::string>();
        ar.save(dump);
    }
    else
    {
        sol::error err = result;
        throw err;
    }
}

void serial_manager::load(sol::table& data, fs::path input_path)
{
    std::ifstream in{input_path.native(), std::ios::binary};
    if (in.fail())
    {
        throw std::runtime_error(
            u8"Error: fail to read "s
            + filesystem::make_preferred_path_in_utf8(input_path));
    }
    putit::binary_iarchive ar{in};
    std::string raw_data;
    ar.load(raw_data);

    serial_env["_TO_DESERIALIZE"] = raw_data;
    auto result = lua->get_state()->safe_script(R"(return Serial.load(_TO_DESERIALIZE))", serial_env);
    serial_env["_TO_DESERIALIZE"] = sol::lua_nil;

    if (result.valid())
    {
        data = result.get<sol::table>();
    }
    else
    {
        sol::error err = result;
        throw err;
    }
}

} // namespace lua
} // namespace elona
