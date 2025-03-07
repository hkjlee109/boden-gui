#include "image_view.hpp"
#include <boden/renderer.hpp>

namespace boden {

image_view_t::image_view_t()
    : view_t(),
      _bounds{0, 0, 0, 0},
      _frame{0, 0, 0, 0}
{
}

image_view_t::~image_view_t()
{
}

image_view_t::image_view_t(boden::layout::rect_t frame)
    : view_t(),
      _bounds{0, 0, frame.size.width, frame.size.height},
      _frame{frame}
{
}

void image_view_t::draw(boden::context_t &ctx)
{
}

} // boden
