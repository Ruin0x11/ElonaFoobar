#pragma once
#include <iosfwd>
#include "../hcl.hpp"
#include "../optional.hpp"

namespace elona
{
namespace snail
{
enum class Key;
}

struct Keybind;
class KeybindManager;

class KeybindSerializer
{
public:
    KeybindSerializer(KeybindManager& keybind_manager)
        : _keybind_manager(keybind_manager)
    {
    }

    void save(std::ostream& out);

private:
    optional<std::string> _serialize_keybind(const Keybind& keybind);
    optional<hcl::Object> KeybindSerializer::_serialize_joystick_binding(
        int joystick_button);
    KeybindManager& _keybind_manager;
};

} // namespace elona
