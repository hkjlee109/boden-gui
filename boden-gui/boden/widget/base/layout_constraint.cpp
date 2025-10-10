#include "layout_constraint.hpp"

namespace boden {
namespace widget {
namespace base {

layout_constraint_t::layout_constraint_t(boden::widget::view_t *view_1,
                                         boden::widget::base::layout_attribute_t attr_1,
                                         boden::widget::base::layout_relation_t relation,
                                         boden::widget::view_t *view_2,
                                         boden::widget::base::layout_attribute_t attr_2,
                                         float multiplier,
                                         float constant)
{
}

layout_constraint_t::~layout_constraint_t()
{
}

} // base
} // widget
} // boden
