#include "layout_x_axis_anchor.hpp"

namespace boden {
namespace widget {
namespace layout {


layout_x_axis_anchor_ref_t layout_x_axis_anchor_t::alloc()
{
    auto instance = std::make_shared<layout_x_axis_anchor_t>();
    instance->init();
    return instance;
}

layout_x_axis_anchor_t::layout_x_axis_anchor_t()
{
}

layout_x_axis_anchor_t::~layout_x_axis_anchor_t()
{
}

void layout_x_axis_anchor_t::init()
{
}

} // layout
} // widget
} // boden
