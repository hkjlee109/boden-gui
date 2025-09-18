#include "hover_button.hpp"

namespace app {
namespace components {

hover_button_t::hover_button_t()
    : boden::widget::button_t()
{
}

hover_button_t::hover_button_t(const boden::layout::rect_t &frame)
    : boden::widget::button_t(frame)
{
}

hover_button_t::~hover_button_t()
{
}

void hover_button_t::mouse_entered(const boden::event_t &ev)
{
    layer.background_color = {0x37, 0x37, 0x37, 0xFF};
    set_needs_display(true);
}

void hover_button_t::mouse_exited(const boden::event_t &ev)
{
    layer.background_color = {0x2A, 0x2A, 0x2A, 0xFF};
    set_needs_display(true);
}

void hover_button_t::set_hover_enabled(bool enabled)
{
    if(enabled)
    {
        auto options{boden::widget::base::tracking_area_t::options::mouse_entered_and_exited};   
        auto origin{convert_point_to_view({0, 0}, nullptr)};
        auto frame{boden::layout::rect_t{origin, _frame.size}};
        _tracking_area = std::make_shared<boden::widget::base::tracking_area_t>(frame,
                                                                                options,
                                                                                shared_from_this());
        add_tracking_area(_tracking_area);
    } 
    else 
    {
        remove_tracking_area(_tracking_area);
        _tracking_area.reset();
    }
}

} // components
} // app
