#include "dimension.hpp"

namespace boden {
namespace widget {
namespace layout {

dimension_ref_t dimension_t::alloc(boden::widget::view_ref_t view, 
                                   boden::widget::layout::attribute_t attr)
{
    auto instance = std::make_shared<dimension_t>(view, attr);
    instance->init();
    return instance;
}

dimension_t::dimension_t(boden::widget::view_ref_t view, boden::widget::layout::attribute_t attr)
    : boden::widget::layout::anchor_t<dimension_t>(view, attr)
{
}

dimension_t::~dimension_t()
{
}

boden::widget::layout::constraint_ref_t dimension_t::constraint_equal_to_constant(float constant)
{
    auto first_item = _view.lock();
    auto second_item = first_item->get_superview();

    return std::make_shared<constraint_t>(first_item,
                                          _attr,
                                          boden::widget::layout::relation_t::equal,
                                          second_item,
                                          _attr,
                                          1.0f,
                                          constant);
}

void dimension_t::init()
{
    boden::widget::layout::anchor_t<dimension_t>::init();
}

} // layout
} // widget
} // boden
