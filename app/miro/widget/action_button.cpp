#include "action_button.hpp"

#include <miro/cursor_type.hpp>
#include <miro/theme/color.hpp>

namespace miro {
namespace widget {

std::shared_ptr<action_button_t> action_button_t::alloc(const boden::layout::rect_t &frame)
{
    auto instance = std::make_shared<action_button_t>(frame);
    instance->init(frame);
    return instance;
}

action_button_t::action_button_t()
    : boden::widget::button_t(),
      _mouse_entered{false},
      _selected{false}
{
}

action_button_t::action_button_t(const boden::layout::rect_t &frame)
    : boden::widget::button_t(frame),
      _mouse_entered{false},
      _selected{false}
{
}

action_button_t::~action_button_t()
{
}

void action_button_t::mouse_entered(const boden::event_t &ev)
{
    _mouse_entered = true;
    update_background_color();

    boden::system_event_t event;
    event.type = static_cast<uint32_t>(boden::system_event_type_t::set_cursor_override);
    event.params["type"] = static_cast<uint8_t>(miro::cursor_type_t::pointing_hand);
    enqueue_system_event(event);

}

void action_button_t::mouse_exited(const boden::event_t &ev)
{
    _mouse_entered = false;
    update_background_color();
    
    boden::system_event_t event;
    event.type = static_cast<uint32_t>(boden::system_event_type_t::set_cursor_override);
    event.params["type"] = static_cast<uint8_t>(miro::cursor_type_t::none);
    enqueue_system_event(event);
}

bool action_button_t::is_selected() const
{
    return _selected;
}
    
void action_button_t::set_selected(bool selected)
{
    _selected = selected;
    set_content_tint_color(_selected ? miro::theme::color::secondary
                                     : miro::theme::color::control_accent);
    
    update_background_color();
}

void action_button_t::set_tracking_enabled(bool enabled)
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

void action_button_t::update_background_color()
{
    if(_mouse_entered)
    {
        get_layer()->set_background_color(miro::theme::color::control_highlight);
    }
    else if(_selected)
    {
        get_layer()->set_background_color(miro::theme::color::secondary_light);
    }
    else
    {
        get_layer()->set_background_color(miro::theme::color::control_background);
    }
    
    set_needs_display(true);
}

void action_button_t::init(const boden::layout::rect_t &frame)
{
    boden::widget::button_t::init(frame);
    _layer->set_background_color({0x00, 0xFF, 0x00, 0xFF});
}

} // widget
} // miro
