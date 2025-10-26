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
    
    if(layer.tid == 0)
    {
        return;
    }
    
    boden::layout::rect_t frame_in_window = convert_rect_to_view(_bounds, nullptr);

    builder.begin(layer.tid, frame_in_window, dirty_rect);

    builder.add_text(_text,
                     {_bounds.origin.x, _bounds.origin.y}, 
                     {_bounds.origin.x + _bounds.size.width, _bounds.origin.y + _bounds.size.height},
                     _text_color);
    
    builder.end();
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
