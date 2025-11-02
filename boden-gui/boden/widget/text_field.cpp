#include "text_field.hpp"

#include <boden/graphic/compositing_operation.hpp>

namespace boden {
namespace widget {

std::shared_ptr<text_field_t> text_field_t::alloc(const boden::layout::rect_t &frame)
{
    auto instance = std::make_shared<text_field_t>(frame);
    instance->init(frame);
    return instance;
}

text_field_t::text_field_t()
    : control_t(),
      _text{},
      _text_color{0xFF, 0xFF, 0xFF, 0xFF},
      _text_alignment{boden::widget::base::text_alignment_t::left}
{
    _enabled = false;
}

text_field_t::text_field_t(const boden::layout::rect_t &frame)
    : control_t{frame},
      _text{},
      _text_color{0xFF, 0xFF, 0xFF, 0xFF},
      _text_alignment{boden::widget::base::text_alignment_t::left}
{
    _enabled = false;
}

text_field_t::~text_field_t()
{
}

bool text_field_t::accepts_first_responder() 
{
    return _enabled;
}

void text_field_t::draw_rect(boden::builder_t &builder, const boden::layout::rect_t &dirty_rect)
{
    boden::widget::view_t::draw_rect(builder, dirty_rect);
    
    if(_hidden) 
    {
        return;
    }
    
    if(_tid == 0)
    {
        return;
    }

    if(_text.empty())
    {
        return;
    }

    boden::layout::rect_t frame_in_window = convert_rect_to_view(_bounds, nullptr);

    builder.begin(_tid, convert_rect_to_view(_bounds, nullptr), dirty_rect, boden::graphic::compositing_operation_t::copy);
    
    float padding = 5;
    builder.add_text(_text,
                     {_bounds.origin.x + padding, _bounds.origin.y + padding},
                     {_bounds.origin.x + _bounds.size.width - padding, _bounds.origin.y + _bounds.size.height - padding},
                     _text_color);
    
    builder.end();
}

void text_field_t::set_enabled(bool enabled)
{
    if(_enabled == enabled)
    {
        return;
    }

    boden::widget::control_t::set_enabled(enabled);

    if(_enabled)
    {
        boden::layout::point_t origin = convert_point_to_view({0, 0}, nullptr);
        boden::layout::rect_t frame{origin, _frame.size};

        boden::system_event_t event; 
        event.type = static_cast<uint32_t>(boden::system_event_type_t::text_input_begin);
        event.params["text"] = _text;
        event.params["frame"] = frame;
        enqueue_system_event(event);

        if(auto window = _window.lock())
        {
            window->make_first_responder(shared_from_this());
        }
        
        _text = "";
        set_needs_display(true);
    }
    else
    {
        boden::system_event_t event;
        event.type = static_cast<uint32_t>(boden::system_event_type_t::text_input_end);
        enqueue_system_event(event);
    }
}

void text_field_t::system_event(const boden::system_event_t &event)
{
    boden::widget::control_t::system_event(event);

    switch(event.type)
    {
        case (uint32_t)boden::system_event_type_t::text_input_commit:
            _text = std::any_cast<const std::string &>(event.params.at("text"));
            set_enabled(false);
            set_needs_display(true);

            if(auto window = _window.lock())
            {
                window->make_first_responder(nullptr);
            }
            break;

        default:
            break;
    }
}

void text_field_t::view_will_move_to_window(std::shared_ptr<boden::widget::window_t> window)
{
    boden::widget::view_t::view_will_move_to_window(window);

    if(_tid == 0 && _bounds.size.width > 0 && _bounds.size.height > 0)
    {
        _tid = window->create_view_texture(_bounds.size);
    }
}

void text_field_t::set_frame(const boden::layout::rect_t &frame)
{
    boden::widget::view_t::set_frame(frame);

    if(_bounds.size.width > 0 && _bounds.size.height > 0)
    {
        if(auto window = _window.lock())
        {
            if(_tid)
            {
                window->destroy_view_texture(_tid);
            }

            _tid = window->create_view_texture(_bounds.size);
        }
    }
}

void text_field_t::set_text(const std::string &text)
{
    _text = text;
}

void text_field_t::set_text_color(const boden::layout::color_t &color)
{
    _text_color = color;
}

void text_field_t::init(const boden::layout::rect_t &frame)
{
    boden::widget::view_t::init(frame);
}

} // widget
} // boden
