#include "shape.hpp"

#include <boden/layout/point.hpp>
#include <ppt/theme/color.hpp>

namespace ppt {
namespace widget {
namespace shape {

shape_t::shape_t()
    : control_t{},
      _editable{false},
      _text_alignment{boden::widget::base::text_alignment_t::left},
      _text_color{ppt::theme::color::label}
{
}

shape_t::shape_t(const boden::layout::rect_t &frame)
    : control_t{frame},
      _editable{false},
      _text_alignment{boden::widget::base::text_alignment_t::left},
      _text_color{ppt::theme::color::label}
{
}

shape_t::~shape_t()
{
}

bool shape_t::accepts_first_responder() 
{
    return _editable;
}

void shape_t::draw(boden::builder_t &builder) 
{
    if(!_text.empty())
    {
        boden::layout::point_t origin = convert_point_to_view({0, 0}, nullptr);
        boden::layout::rect_t frame{origin, _frame.size};

        builder.push_clip_rect({frame.origin.x, 
                                frame.origin.y, 
                                frame.size.width, 
                                frame.size.height});
    
        float padding = 5;
        builder.add_text(_text,
                         {frame.origin.x + padding, frame.origin.y + padding},
                         {frame.origin.x + frame.size.width - padding, frame.origin.y + frame.size.height - padding},
                         _text_color);
    
        builder.pop_clip_rect();
    }
}

void shape_t::system_event(const boden::system_event_t &event)
{
    switch(event.type)
    {
        case (uint32_t)boden::system_event_type_t::text_input_commit:
            _text = std::any_cast<const std::string &>(event.params.at("text"));
            set_editable(false);
            set_needs_display(true);

            if(auto window = _window.lock())
            {
                printf("# text_input_commit. resigining..\n");
                window->make_first_responder(nullptr);
            }
            break;

        default:
            break;
    }
}

bool shape_t::is_editable() const
{
    return _editable;
}

void shape_t::set_editable(bool editable)
{
    if(_editable == editable)
    {
        return;
    }
    
    _editable = editable;
    if(_editable)
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

const boden::layout::rect_t & shape_t::get_frame_cache() const
{
    return _frame_cache;
}

void shape_t::set_frame_cache(const boden::layout::rect_t& frame)
{
    _frame_cache = frame;
}

void shape_t::set_text(const std::string &text)
{
    _text = text;
}

void shape_t::set_text_color(const boden::layout::color_t &color)
{
    _text_color = color;
}

} // shape
} // widget
} // ppt
