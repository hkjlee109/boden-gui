#pragma once

#include <boden/widget/layout/anchor.hpp>

#include <memory>

namespace boden {
namespace widget {
namespace layout {

class y_axis_anchor_t;
using y_axis_anchor_ref_t = std::shared_ptr<boden::widget::layout::y_axis_anchor_t>;

class y_axis_anchor_t : public anchor_t<y_axis_anchor_t>
{
public:
    static y_axis_anchor_ref_t alloc(boden::widget::view_ref_t view, boden::widget::layout::attribute_t attr);
    
    y_axis_anchor_t(boden::widget::view_ref_t view, boden::widget::layout::attribute_t attr);
    virtual ~y_axis_anchor_t();

protected:
    void init() override;

private:
};

} // layout
} // widget
} // boden
