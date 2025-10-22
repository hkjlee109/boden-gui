#include "label.hpp"
#include <boden/renderer.hpp>

namespace boden {
namespace widget {

label_t::label_t()
    : view_t(),
      _text{},
      _text_color{0xFF, 0xFF, 0xFF, 0xFF}
{
}

label_t::label_t(const boden::layout::rect_t &frame)
    : view_t{frame},
      _text{},
      _text_color{0xFF, 0xFF, 0xFF, 0xFF},
      _text_alignment{boden::widget::base::text_alignment_t::left}
{
}

label_t::~label_t()
{
}

void label_t::draw_rect(boden::builder_t &builder, const boden::layout::rect_t &dirty_rect)
{
    if(_hidden) 
    {
        return;
    }
    
    boden::layout::point_t origin = convert_point_to_view({0, 0}, nullptr);
    boden::layout::rect_t frame{origin, _frame.size};
    boden::layout::rect_t clip_rect{origin + dirty_rect.origin, dirty_rect.size};
        
    builder.push_clip_rect(clip_rect);
    
    builder.add_text(_text,
                     {frame.origin.x, frame.origin.y}, 
                     {frame.origin.x + frame.size.width, frame.origin.y + frame.size.height},
                     _text_color);

    builder.pop_clip_rect();
}

void label_t::set_text(const std::string &text)
{
    _text = text;
}

void label_t::set_text_color(const boden::layout::color_t &color)
{
    _text_color = color;
}

} // widget
} // boden
