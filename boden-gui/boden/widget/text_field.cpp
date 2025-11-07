#include "text_field.hpp"

#include <boden/graphic/compositing_operation.hpp>
#include <boden/widget/constant.hpp>

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
      _text_alignment{boden::widget::base::text_alignment_t::left}
{
    _enabled = false;
}

text_field_t::text_field_t(const boden::layout::rect_t &frame)
    : control_t{frame},
      _text_alignment{boden::widget::base::text_alignment_t::left}
{
    _enabled = false;
}

text_field_t::~text_field_t()
{
    auto tid = _text_layer->get_texture_id();
    if(tid)
    {
        if(auto window = _window.lock()) 
        {
            window->destroy_view_texture(tid);
        }
    }
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

    auto frame_in_window = convert_rect_to_view(_bounds, nullptr);
    _text_layer->draw(builder, frame_in_window);
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
        event.params["text"] = get_text();
        event.params["frame"] = frame;
        enqueue_system_event(event);

        if(auto window = _window.lock())
        {
            window->make_first_responder(shared_from_this());
        }
        
        set_text("");
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
            set_text(std::any_cast<const std::string &>(event.params.at("text")));
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
    create_text_layer_texture();
}

void text_field_t::set_frame(const boden::layout::rect_t &frame)
{
    boden::widget::view_t::set_frame(frame);
    create_text_layer_texture();
}

const std::string & text_field_t::get_text() const
{
    return _text_layer->get_text();
}

void text_field_t::set_text(const std::string &text)
{
    _text_layer->set_text(text);
}

void text_field_t::set_text_color(const boden::layout::color_t &color)
{
    _text_layer->set_text_color(color);
}

void text_field_t::init(const boden::layout::rect_t &frame)
{
    boden::widget::view_t::init(frame);

    _text_layer = boden::widget::layer::text_layer_t::alloc({0, 0, frame.size.width, frame.size.height});
    _layer->add_layer(_text_layer);
}

void text_field_t::create_text_layer_texture()
{
    auto window = _window.lock();
    if(!window)
    {
        return;
    }

    auto tid = _text_layer->get_texture_id();
    if(tid)
    {
        window->destroy_view_texture(tid);
    }

    _text_layer->set_texture_id(window->create_view_texture(_text_layer->get_frame().size));
}

} // widget
} // boden
