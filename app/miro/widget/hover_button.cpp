#include "hover_button.hpp"

#include <miro/theme/color.hpp>

namespace miro {
namespace widget {

std::shared_ptr<hover_button_t> hover_button_t::alloc(const boden::layout::rect_t &frame)
{
    auto instance = std::make_shared<hover_button_t>(frame);
    instance->init(frame);
    return instance;
}

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
    get_layer()->set_background_color(miro::theme::color::control_highlight);
    set_needs_display(true);
}

void hover_button_t::mouse_exited(const boden::event_t &ev)
{
    get_layer()->set_background_color(miro::theme::color::control_background);
    set_needs_display(true);
}

void hover_button_t::set_hover_enabled(bool enabled)
{
    if(enabled)
    {
        auto options{boden::widget::base::tracking_area_t::options::mouse_entered_and_exited};
        boden::layout::rect_t bounds{0, 0, _frame.size.width, _frame.size.height};
        _tracking_area = std::make_shared<boden::widget::base::tracking_area_t>(bounds,
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

void hover_button_t::init(const boden::layout::rect_t &frame)
{
    boden::widget::view_t::init(frame);
}

} // widget
} // miro
