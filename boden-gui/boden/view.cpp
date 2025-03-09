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

} // boden
