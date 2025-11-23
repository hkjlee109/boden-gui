#include "clip_view.hpp"

namespace boden {
namespace widget {

std::shared_ptr<clip_view_t> clip_view_t::alloc(const boden::layout::rect_t &frame)
{
    auto instance = std::make_shared<clip_view_t>(frame);
    instance->init(frame);
    return instance;
}

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

void clip_view_t::init(const boden::layout::rect_t &frame)
{
    boden::widget::view_t::init(frame);
}

} // widget
} // boden
