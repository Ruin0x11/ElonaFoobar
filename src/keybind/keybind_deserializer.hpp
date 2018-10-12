#pragma once
#include <iosfwd>
#include "../hcl.hpp"
#include "../optional.hpp"
#include "../snail/input.hpp"

namespace elona
{
struct Keybind;
class KeybindManager;

class KeybindDeserializer
{
public:
    KeybindDeserializer(KeybindManager& keybind_manager)
        : _keybind_manager(keybind_manager)
    {
    }

    void load(std::istream& in);

private:
    optional<Keybind> _deserialize_keybind(
        const hcl::Value& object,
        const std::string& id);
    optional<int> _deserialize_joystick_binding(
        const hcl::Value& object,
        const std::string& id);

    void visit_object(const hcl::Object& object);
    void visit_keybinding(
        const std::string& action_id,
        const hcl::Value& object);

    KeybindManager& _keybind_manager;
};

} // namespace elona
