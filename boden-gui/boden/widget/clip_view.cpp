#include "clip_view.hpp"

namespace boden {
namespace widget {

clip_view_t::clip_view_t()
    : boden::widget::view_t{}
{
}

clip_view_t::clip_view_t(const boden::layout::rect_t &frame)
    : boden::widget::view_t{frame}
{
}

clip_view_t::~clip_view_t()
{
}

void clip_view_t::scroll(const boden::layout::point_t &point)
{
    
}

} // widget
} // boden
