#include "keybind_deserializer.hpp"
#include <iostream>
#include <string>
#include "../hcl.hpp"
#include "keybind.hpp"
#include "keybind_manager.hpp"

namespace elona
{

void KeybindDeserializer::load(std::istream& in)
{
    auto parsed = hclutil::load(in);
    const auto& value = hclutil::skip_sections(
        parsed, {"keybindings", "core"}, "[input stream]");

    visit_object(value.as<hcl::Object>());
}

void KeybindDeserializer::visit_object(const hcl::Object& object)
{
    for (const auto& pair : object)
    {
        const auto& key = pair.first;
        const auto& value = pair.second;
        if (value.is<hcl::Object>())
        {
            visit_keybinding(key, value);
        }
    }
}

optional<Keybind> KeybindDeserializer::_deserialize_keybind(
    const hcl::Value& object,
    const std::string& id)
{
    const hcl::Value* val = object.find(id);
    if (val && val->is<std::string>())
    {
        if (auto keybind = Keybind::from_string(val->as<std::string>()))
        {
            return keybind;
        }
    }

    return none;
}

optional<int> KeybindDeserializer::_deserialize_joystick_binding(
    const hcl::Value& object,
    const std::string& id)
{
    const hcl::Value* val = object.find(id);
    if (val && val->is<hcl::Object>())
    {
        const hcl::Value* button = val->find("button");
        if (button && button->is<int>())
        {
            int val = button->as<int>();
            if (val > -1)
            {
                return val;
            }
        }
    }

    return none;
}

void KeybindDeserializer::visit_keybinding(
    const std::string& action_id,
    const hcl::Value& object)
{
    if (!_keybind_manager.is_registered(action_id))
    {
        return;
    }

    auto& binding = _keybind_manager.binding(action_id);

    if (auto keybind = _deserialize_keybind(object, "primary"))
    {
        binding.primary = *keybind;
    }

    if (auto keybind = _deserialize_keybind(object, "alternate"))
    {
        binding.alternate = *keybind;
    }

    if (auto joystick_button =
            _deserialize_joystick_binding(object, "joystick"))
    {
        binding.joystick_button = *joystick_button;
    }
}

} // namespace elona
