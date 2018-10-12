#include "keybind_serializer.hpp"
#include <iostream>
#include "../hcl.hpp"
#include "keybind.hpp"
#include "keybind_manager.hpp"

namespace elona
{

void KeybindSerializer::save(std::ostream& out)
{
    // Create a top level "keybindings" section.
    hcl::Value keybindings = hcl::Value(hcl::Object());
    keybindings.set("keybindings", hcl::Object());
    hcl::Value* parent = keybindings.find("keybindings");
    assert(parent);

    // Create "core" section.
    parent->set("core", hcl::Object());
    parent = parent->find("core");
    assert(parent);

    // Create sections under the top-level "keybindings" section for each mod
    // that has keybindings (for now, only "core"), then write their individual
    // config sections.
    for (const auto& pair : _keybind_manager)
    {
        const auto& action_id = pair.first;
        const auto& binding = pair.second;

        // Add a new section for this action.
        hcl::Object object;
        bool valid = false;

        // Set primary, alternate and joystick bindings.
        if (auto str = _serialize_keybind(binding.primary))
        {
            object.emplace("primary", *str);
            valid = true;
        }
        if (auto str = _serialize_keybind(binding.alternate))
        {
            object.emplace("alternate", *str);
            valid = true;
        }
        if (auto obj = _serialize_joystick_binding(binding.joystick_button))
        {
            object.emplace("joystick", *obj);
            valid = true;
        }

        if (valid)
        {
            parent->set(action_id, object);
        }
    }

    out << keybindings;
}

optional<std::string> KeybindSerializer::_serialize_keybind(
    const Keybind& keybind)
{
    if (!keybind_is_bindable_key(keybind.main))
    {
        return none;
    }

    return keybind.to_string();
}

optional<hcl::Object> KeybindSerializer::_serialize_joystick_binding(
    int joystick_button)
{
    return hcl::Object{{"button", joystick_button}};
}

} // namespace elona
