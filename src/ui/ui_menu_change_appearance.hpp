#pragma once
#include "ui_menu.hpp"

namespace elona
{
namespace ui
{

struct pcc_info
{
    enum class item_type
    {
        confirm,
        change_page,
        pcc_part
    };

    item_type item;
    std::string name;
    int index;
    int current_value;
    bool use_external_image;
};

class ui_menu_change_appearance : public ui_menu<bool>
{
public:
    enum class operation
    {
        normal,
        character_making
    };

public:
    ui_menu_change_appearance(operation operation)
        : _operation(operation)
    {
    }

protected:
    virtual bool init();
    virtual void update();
    virtual void draw();
    virtual optional<ui_menu_change_appearance::result_type> on_key(
        const std::string& key);

private:
    operation _operation;

    int _frame = 0;
    int _pcc_facing_direction = 0;
};

} // namespace ui
} // namespace elona
