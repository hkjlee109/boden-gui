#include "button.hpp"
#include <boden/renderer.hpp>

namespace boden {

button_t::button_t()
    : view_t(),
      _bounds{0, 0, 0, 0},
      _frame{0, 0, 0, 0}
{
}

button_t::~button_t()
{
}

button_t::button_t(boden::geometry::rect_t frame)
    : view_t(),
      _bounds{0, 0, frame.size.width, frame.size.height},
      _frame{frame}
{
}

void button_t::draw(boden::context_t &ctx)
{

}

} // boden
