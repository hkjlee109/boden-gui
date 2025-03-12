#include "view.hpp"

namespace boden {

view_t::view_t()
    : _bounds{0, 0, 0, 0},
      _frame{0, 0, 0, 0}
{
}

view_t::view_t(const boden::layout::rect_t &frame)
    : _bounds{0, 0, frame.size.width, frame.size.height},
      _frame{frame}
{
}

view_t::~view_t()
{
}

float view_t::get_layer_border_width() const
{
    return _layer.border_width;
}

void view_t::set_layer_border_width(float width)
{
    _layer.border_width = width;
}

const boden::layout::color_t & view_t::get_layer_border_color() const 
{
    return _layer.border_color;
}

} // boden
