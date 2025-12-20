#include "x_axis_anchor.hpp"

namespace boden {
namespace widget {
namespace layout {

x_axis_anchor_ref_t x_axis_anchor_t::alloc(boden::widget::view_ref_t view, 
                                           boden::widget::layout::attribute_t attr)
{
    auto instance = std::make_shared<x_axis_anchor_t>(view, attr);
    instance->init();
    return instance;
}

x_axis_anchor_t::x_axis_anchor_t(boden::widget::view_ref_t view, boden::widget::layout::attribute_t attr)
    : boden::widget::layout::anchor_t<x_axis_anchor_t>(view, attr)
{
}

x_axis_anchor_t::~x_axis_anchor_t()
{
}

void x_axis_anchor_t::init()
{
    boden::widget::layout::anchor_t<x_axis_anchor_t>::init();
}

} // layout
} // widget
} // boden
