#pragma once

#include <boden/widget/layout/anchor.hpp>

#include <memory>

namespace boden {
namespace widget {
namespace layout {

class x_axis_anchor_t;
using x_axis_anchor_ref_t = std::shared_ptr<boden::widget::layout::x_axis_anchor_t>;

class x_axis_anchor_t : public anchor_t<x_axis_anchor_t>
{
public:
    static x_axis_anchor_ref_t alloc(boden::widget::view_ref_t view, boden::widget::layout::attribute_t attr);
    
    x_axis_anchor_t(boden::widget::view_ref_t view, boden::widget::layout::attribute_t attr);
    virtual ~x_axis_anchor_t();

protected:
    void init() override;

private:
};

} // layout
} // widget
} // boden
