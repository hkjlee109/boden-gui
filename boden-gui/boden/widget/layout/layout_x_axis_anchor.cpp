#include "layout_x_axis_anchor.hpp"

namespace boden {
namespace widget {
namespace layout {

layout_x_axis_anchor_ref_t layout_x_axis_anchor_t::alloc(boden::widget::view_ref_t view, boden::widget::layout::layout_attribute_t attr)
{
    auto instance = std::make_shared<layout_x_axis_anchor_t>(view, attr);
    instance->init();
    return instance;
}

layout_x_axis_anchor_t::layout_x_axis_anchor_t(boden::widget::view_ref_t view, boden::widget::layout::layout_attribute_t attr)
    : boden::widget::layout::layout_anchor_t<layout_x_axis_anchor_t>(view, attr)
{
}

layout_x_axis_anchor_t::~layout_x_axis_anchor_t()
{
}

void layout_x_axis_anchor_t::init()
{
    boden::widget::layout::layout_anchor_t<layout_x_axis_anchor_t>::init();
}

} // layout
} // widget
} // boden
