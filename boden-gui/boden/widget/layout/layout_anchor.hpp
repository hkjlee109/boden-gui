#pragma once

#include <boden/widget/layout/layout_attribute.hpp>
#include <boden/widget/layout/layout_constraint.hpp>
#include <boden/widget/view.hpp>

#include <memory>

namespace boden {
namespace widget {
namespace layout {

template<typename T>
class layout_anchor_t
{
public:
    layout_anchor_t(boden::widget::view_ref_t view, boden::widget::layout::layout_attribute_t attr)
    {
        _view = view;
        _attr = attr;
    }

    virtual ~layout_anchor_t()
    {
    }

    boden::widget::layout::layout_constraint_ref_t constraint_equal_to_anchor(const std::shared_ptr<layout_anchor_t<T>> &anchor)
    {
        return std::make_shared<layout_constraint_t>(_view.lock(),
                                                     _attr,
                                                     layout_relation_t::equal,
                                                     anchor->get_view(),
                                                     anchor->get_attrubute(),
                                                     1.0f,
                                                     0.0f);
    }

    boden::widget::layout::layout_attribute_t get_attrubute() const
    {
        return _attr;
    }

    boden::widget::view_ref_t get_view() const
    {
        return _view.lock();;
    }

protected:
    virtual void init()
    {
    }

    boden::widget::layout::layout_attribute_t _attr;
    boden::widget::view_wref_t _view;

private:
};

} // layout
} // widget
} // boden
