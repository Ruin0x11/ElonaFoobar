#pragma once



namespace elona
{

enum class curse_state_t
{
    doomed,
    cursed,
    none,
    blessed,
};


inline bool is_cursed(curse_state_t s)
{
    return s <= curse_state_t::cursed;
}


enum class identification_state_t
{
    unidentified,
    partly_identified,
    almost_identified,
    completely_identified,
};

enum class dialog_type_t
{
    info_ok = 0,
    warning_ok = 1,
    info_yes_no = 2,
    warning_yes_no = 3,
    open_file = 16,
    save_as = 17,
    color_selection = 32,
    color_selection_with_matrix = 33,
};

} // namespace elona
