#pragma once



namespace elona
{

const constexpr char* data_id_separator = "#";


#define DATA_REQ(name, type) type name = data.required<type>(#name);
#define DATA_REQ_FUNC(name) \
    sol::protected_function name##_ = \
        data.required<sol::protected_function>(#name); \
    lua::WrappedFunction name(id, name##_);
#define DATA_REQ_ID(name) \
    SharedId name##_ = data.required<std::string>(#name); \
    SharedId name(name##_);
#define DATA_OPT_OR(name, type, def) \
    type name = data.optional_or<type>(#name, def);
#define DATA_OPT(name, type) optional<type> name = data.optional<type>(#name);
#define DATA_OPT_FUNC(name) \
    optional<sol::protected_function> name##_ = \
        data.optional<sol::protected_function>(#name); \
    optional<lua::WrappedFunction> name = none; \
    if (name##_) \
    { \
        name = lua::WrappedFunction(id, *name##_); \
    }
#define DATA_OPT_ID(name) \
    optional<std::string> name##_ = data.optional<std::string>(#name); \
    optional<SharedId> name = none; \
    if (name##_) \
    { \
        name = SharedId(*name##_); \
    }
#define DATA_VEC(name, type) std::vector<type> name = data.vector<type>(#name);
#define DATA_TABLE(name, keytype, valuetype) \
    std::unordered_map<keytype, valuetype> name = \
        data.unordered_map<keytype, valuetype>(#name);
#define DATA_ENUM(name, type, table, def) \
    type name = data.enum_or<type>(#name, lua::LuaEnums::table, def);

} // namespace elona
