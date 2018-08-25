#pragma once
#include "ui_menu.hpp"

namespace elona
{
namespace ui
{

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
};

} // namespace ui
} // namespace elona
