#pragma once

#include <boden/widget/view.hpp>
#include <boden/widget/layout/layout_attribute.hpp>
#include <boden/widget/layout/layout_relation.hpp>

namespace boden {
namespace widget {
namespace layout {

class layout_constraint_t;
using layout_constraint_ref_t = std::shared_ptr<boden::widget::layout::layout_constraint_t>;

class layout_constraint_t
{
public:
    layout_constraint_t(boden::widget::view_ref_t *view_1,
                        boden::widget::layout::layout_attribute_t attr_1,
                        boden::widget::layout::layout_relation_t relation,
                        boden::widget::view_ref_t *view_2,
                        boden::widget::layout::layout_attribute_t attr_2,
                        float multiplier,
                        float constant);
    ~layout_constraint_t();

    bool is_active() const;
    void set_active(bool active);

private:
    bool _active;
};

} // layout
} // widget
} // boden
