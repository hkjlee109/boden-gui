#pragma once

#include <boden/widget/layout/anchor.hpp>

#include <memory>

namespace boden {
namespace widget {
namespace layout {

class dimension_t;
using dimension_ref_t = std::shared_ptr<boden::widget::layout::dimension_t>;

class dimension_t : public anchor_t<dimension_t>
{
public:
    static dimension_ref_t alloc(boden::widget::view_ref_t view, boden::widget::layout::attribute_t attr);
    
    dimension_t(boden::widget::view_ref_t view, boden::widget::layout::attribute_t attr);
    virtual ~dimension_t();

    boden::widget::layout::constraint_ref_t constraint_equal_to_constant(float constant);

protected:
    void init() override;

private:
};

} // layout
} // widget
} // boden
