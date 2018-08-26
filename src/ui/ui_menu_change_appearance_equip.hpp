#pragma once
#include "ui_menu.hpp"

namespace elona
{
namespace ui
{

class ui_menu_change_appearance_equip : public ui_menu<dummy_result>
{
public:
    ui_menu_change_appearance_equip()
    {
    }

protected:
    virtual bool init();
    virtual void update();
    virtual void draw();
    virtual optional<ui_menu_change_appearance_equip::result_type> on_key(
        const std::string& key);

private:
    int _frame = 0;
    int _pcc_facing_direction = 0;
};

} // namespace ui
} // namespace elona
