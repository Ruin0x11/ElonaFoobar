#pragma once
#include "keybind.hpp"

namespace elona
{

class InputContext
{
public:
    void enable_category(ActionCategory category)
    {
        _excluded_categories.erase(category);
    }

    void disable_category(ActionCategory category)
    {
        _excluded_categories.insert(category);
    }

    /**
     * Queries and sets input by translating raw input into a game action
     * through a keybinding.
     *
     * @param delay_type Type of delay to use when holding keys.
     *
     * @return An action if one could be mapped from input, none otherwise.
     */
    std::string check_for_command(
        KeyWaitDelay delay_type = KeyWaitDelay::always);

    /**
     * Queries and translates input into an action, or sets list_index to a
     * number other than -1 based on the size of the global list/listn.
     * Keybindings are given higher priority than the list selection keys.
     *
     * @param list_index A list index to use.
     *
     * @return An action if one could be mapped from input, none otherwise.
     */
    std::string check_for_command_with_list(int& list_index);

    /**
     * Resets this context and the pressed input keys so keys that are held at
     * this point will no longer be counted as pressed.
     */
    void reset();

    static InputContext create(InputContextType type);
    static InputContext& instance();
    static InputContext& for_menu();

private:
    /**
     * Adds all actions that are a part of the given category.
     */
    void _add_actions_from_category(ActionCategory category);

    optional<MatchedInput> _matches(
        const std::string& action_id,
        snail::ModKey modifiers);

    optional<std::string> _check_movement_action(
        const std::set<snail::Key>& keys,
        snail::ModKey modifiers);

    bool _is_nonmovement_key(const snail::Key& k);

    optional<MatchedInput> _check_normal_action(snail::ModKey modifiers);

    std::string _delay_movement_action(
        const std::string& action,
        snail::ModKey modifiers,
        KeyWaitDelay delay_type);

    bool _delay_normal_action(const MatchedInput& input);


    std::set<std::string> _available_actions;
    std::vector<std::string> _available_actions_sorted;
    std::unordered_set<ActionCategory> _excluded_categories;
    std::string _last_action;
    int _last_action_held_frames{};
};

} // namespace elona
