#include "layout_constraint.hpp"

namespace boden {
namespace widget {
namespace layout {

layout_constraint_t::layout_constraint_t(boden::widget::view_ref_t *view_1,
                                         boden::widget::layout::layout_attribute_t attr_1,
                                         boden::widget::layout::layout_relation_t relation,
                                         boden::widget::view_ref_t *view_2,
                                         boden::widget::layout::layout_attribute_t attr_2,
                                         float multiplier,
                                         float constant)
    : _active{false}
{
}

layout_constraint_t::~layout_constraint_t()
{
}

bool layout_constraint_t::is_active() const
{
    return _active;
}
    
void layout_constraint_t::set_active(bool active)
{
    _active = active;
}

} // layout
} // widget
} // boden
