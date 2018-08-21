#include "hcl_parsing_native.hpp"
#include "../hcl.hpp"

namespace elona
{
namespace lua
{

static void
_parse_value(sol::state&, sol::table&, const sol::object&, const hcl::Value&);

static void
_parse_object(sol::state& lua, sol::table& table, const hcl::Object& object)
{
    for (const auto& it : object)
    {
        const auto& key = it.first;
        const auto& value = it.second;

        _parse_value(lua, table, sol::make_object(lua, key), value);
    }
}

static void _parse_value(
    sol::state& lua,
    sol::table& table,
    const sol::object& key,
    const hcl::Value& value)
{
    switch (value.type())
    {
    case hcl::Value::Type::NULL_TYPE: return;
    case hcl::Value::Type::BOOL_TYPE: table[key] = value.as<bool>(); break;
    case hcl::Value::Type::INT_TYPE: table[key] = value.as<int>(); break;
    case hcl::Value::Type::DOUBLE_TYPE: table[key] = value.as<double>(); break;
    case hcl::Value::Type::STRING_TYPE:
    case hcl::Value::Type::IDENT_TYPE:
    case hcl::Value::Type::HIL_TYPE:
        table[key] = value.as<std::string>();
        break;
    case hcl::Value::Type::LIST_TYPE:
    {
        // Lua tables are 1-indexed.
        int index = 1;
        sol::table list_table = table.create_named(key);
        for (const auto& list_value : value.as<hcl::List>())
        {
            _parse_value(
                lua, list_table, sol::make_object(lua, index), list_value);
            index++;
        }
    }
    break;
    case hcl::Value::Type::OBJECT_TYPE:
        sol::table object_table = table.create_named(key);
        _parse_object(lua, object_table, value.as<hcl::Object>());
        break;
    }
}

sol::table parse_hcl_native(const std::string& filename, sol::state& lua)
{
    auto value = hclutil::load(filename);
    sol::table table = lua.create_table();
    if (!value.is<hcl::Object>())
    {
        throw std::runtime_error(
            filename + ": HCL object not found at top level");
    }
    _parse_object(lua, table, value.as<hcl::Object>());
    return table;
}

} // namespace lua
} // namespace elona
