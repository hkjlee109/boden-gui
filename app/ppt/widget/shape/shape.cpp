#include "shape.hpp"

#include <boden/layout/point.hpp>
#include <ppt/theme/color.hpp>

namespace ppt {
namespace widget {
namespace shape {

shape_t::shape_t()
    : control_t{},
      _selected{false},
      _text_alignment{boden::widget::base::text_alignment_t::left},
      _text_color{ppt::theme::color::label}
{
}

shape_t::shape_t(const boden::layout::rect_t &frame)
    : control_t{frame},
      _selected{false},
      _text_alignment{boden::widget::base::text_alignment_t::left},
      _text_color{ppt::theme::color::label}
{
}

shape_t::~shape_t()
{
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

    if(_selected)
    {
        boden::layout::point_t origin = convert_point_to_view({0, 0}, nullptr);
        boden::layout::rect_t frame{origin, _frame.size};

        builder.push_clip_rect({frame.origin.x - HANDLE_SIZE_HALF - 1, 
                                frame.origin.y - HANDLE_SIZE_HALF - 1, 
                                frame.size.width + HANDLE_SIZE + 2, 
                                frame.size.height + HANDLE_SIZE + 2});
    
        boden::layout::point_t pts[] = {
            {frame.origin.x, frame.origin.y},
            {frame.origin.x + frame.size.width, frame.origin.y},
            {frame.origin.x, frame.origin.y + frame.size.height},
            {frame.origin.x + frame.size.width, frame.origin.y + frame.size.height},

            {frame.origin.x + frame.size.width / 2, frame.origin.y},
            {frame.origin.x, frame.origin.y + frame.size.height / 2},
            {frame.origin.x + frame.size.width, frame.origin.y + frame.size.height / 2},
            {frame.origin.x + frame.size.width / 2, frame.origin.y + frame.size.height}
        };

        for (const auto &pt : pts) {
            builder.add_rect_filled({pt.x - HANDLE_SIZE_HALF, pt.y - HANDLE_SIZE_HALF}, 
                                    {pt.x + HANDLE_SIZE_HALF, pt.y + HANDLE_SIZE_HALF},
                                    {0xFF, 0xFF, 0xFF, 0xFF});
            builder.add_rect({pt.x - HANDLE_SIZE_HALF, pt.y - HANDLE_SIZE_HALF}, 
                             {pt.x + HANDLE_SIZE_HALF, pt.y + HANDLE_SIZE_HALF},
                             {0x56, 0x56, 0x56, 0xFF},
                             1);
        }

        builder.pop_clip_rect();
    }
}

void shape_t::system_event(const boden::system_event_t &event)
{
    switch(event.type)
    {
        case (uint32_t)boden::system_event_type_t::text_input_commit:
            _text = std::any_cast<const std::string &>(event.params.at("text"));
            set_needs_display(true);
            break;

        default:
            break;
    }
}

bool shape_t::become_first_responder()
{
    boden::widget::view_t::become_first_responder();

    boden::layout::point_t origin = convert_point_to_view({0, 0}, nullptr);
    boden::layout::rect_t frame{origin, _frame.size};

    boden::system_event_t event; 
    event.type = static_cast<uint32_t>(boden::system_event_type_t::text_input_begin);
    event.params["frame"] = frame;
    enqueue_system_event(event);
    
    return true;
}

bool shape_t::resign_first_responder()
{
    return true;
}

bool shape_t::is_selected() const
{
    return _selected;
}

void shape_t::set_selected(bool selected)
{
    _selected = selected;
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
