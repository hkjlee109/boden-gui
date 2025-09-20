#pragma once

#include <boden/widget/view.hpp>
#include <boden/widget/base/layout_attribute.hpp>
#include <boden/widget/base/layout_relation.hpp>

namespace boden {
namespace widget {
namespace base {

class layout_constraint_t
{
public:
    layout_constraint_t(boden::widget::view_t *view_1,
                        boden::widget::base::layout_attribute_t attr_1,
                        boden::widget::base::layout_relation_t relation,
                        boden::widget::view_t *view_2,
                        boden::widget::base::layout_attribute_t attr_2,
                        float multiplier,
                        float constant);
    ~layout_constraint_t();

private:
};

} // base
} // widget
} // boden
