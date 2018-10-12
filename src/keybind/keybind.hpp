#pragma once
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "../enums.hpp"
#include "../optional.hpp"
#include "../snail/input.hpp"
#include "../thirdparty/ordered_map/ordered_map.h"

using namespace std::literals::string_literals;

namespace elona
{

struct Keybind
{
    Keybind()
        : main(snail::Key::none)
        , modifiers(snail::ModKey::none)
    {
    }

    Keybind(snail::Key main, snail::ModKey modifiers)
        : main(main)
        , modifiers(modifiers)
    {
    }

    snail::Key main;
    snail::ModKey modifiers;

    bool operator==(const Keybind& other) const noexcept
    {
        return main == other.main && modifiers == other.modifiers;
    }

    bool empty() const
    {
        return main == snail::Key::none;
    }

    std::string to_string() const;

    static optional<Keybind> from_string(std::string str);

    void clear()
    {
        main = snail::Key::none;
        modifiers = snail::ModKey::none;
    }
};

// The current system queries input to see if keys bound to an action are
// pressed, instead of checking from a list of pressed keys to see which action
// was activated. Due to this, there needs to be some way to remember the
// key/joystick button that was pressed to handle key delay from holding a
// key/button over multiple frames if a bound action was activated. This is a
// consequence of needing to emulate the original key delay behavior, which
// treated nonmovement input as text input and was delayed based on OS-level key
// repeat settings.
struct MatchedInput
{
    MatchedInput()
    {
    }
    MatchedInput(Keybind keybind)
        : action_id("")
        , keybind(keybind)
    {
    }
    MatchedInput(int joystick_button)
        : action_id("")
        , joystick_button(joystick_button)
    {
    }
    MatchedInput(std::string action_id, Keybind keybind)
        : action_id(action_id)
        , keybind(keybind)
    {
    }
    MatchedInput(std::string action_id, int joystick_button)
        : action_id(action_id)
        , joystick_button(joystick_button)
    {
    }

    void clear()
    {
        action_id = "";
        keybind.clear();
        joystick_button = -1;
    }

    bool matches(const MatchedInput& other)
    {
        return keybind == other.keybind
            && joystick_button == other.joystick_button;
    }

    std::string action_id;
    Keybind keybind{};
    int joystick_button = -1;
};

// clang-format off
enum class InputContextType
{
    menu, // [default, shortcut, selection, menu]
    game  // [default, shortcut, game, wizard]
};
// clang-format on

enum class ActionCategory
{
    default_,
    shortcut,
    selection,
    menu,
    game,
    wizard
};

struct Action
{
    Action(ActionCategory category, std::vector<Keybind> default_keybinds)
        : description_key("")
        , category(category)
        , default_keybinds(default_keybinds)
        , visible(true)
    {
    }

    std::string description_key;
    ActionCategory category;
    std::vector<Keybind> default_keybinds;
    bool visible;
};


typedef tsl::ordered_map<std::string, Action> ActionMap;

namespace keybind
{
extern ActionMap actions;
}


void initialize_keybind_actions(ActionMap& actions);
void initialize_keybindings();

optional<std::string> keybind_key_name(snail::Key key, bool shift = false);
std::string keybind_key_short_name(snail::Key key, bool shift = false);
optional<snail::Key> keybind_key_code(
    const std::string& name,
    bool shift = false);
bool keybind_is_bindable_key(snail::Key key);
bool keybind_action_has_category(
    const std::string& action_id,
    ActionCategory category);
std::unordered_set<ActionCategory> keybind_conflicting_action_categories(
    ActionCategory action_category);

void keybind_regenerate_key_select();
void keybind_regenerate_key_names();

/**
 * For selection and shortcut actions, returns the integer index of the
 * selection item/shortcut.
 *
 * Never to be used for other actions, since it assumes the action ID has the
 * format <prefix>_<number>.
 */
int keybind_index_number(const std::string& action_id);

/**
 * For selection actions, returns the key bound to the selection action of the
 * given index.
 */
snail::Key keybind_selection_key_from_index(int index);

} // namespace elona
