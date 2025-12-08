#pragma once

#include <boden/widget/layout/layout_anchor.hpp>

#include <memory>

namespace boden {
namespace widget {
namespace layout {

class layout_x_axis_anchor_t;
using layout_x_axis_anchor_ref_t = std::shared_ptr<boden::widget::layout::layout_x_axis_anchor_t>;

class layout_x_axis_anchor_t : public layout_anchor_t<layout_x_axis_anchor_t>
{
public:
    static layout_x_axis_anchor_ref_t alloc();
    
    layout_x_axis_anchor_t();
    virtual ~layout_x_axis_anchor_t();

protected:
    void init() override;

private:

};

} // layout
} // widget
} // boden
