#pragma once

#include <boden/widget/layout/attribute.hpp>
#include <boden/widget/layout/constraint.hpp>
#include <boden/widget/layout/relation.hpp>
#include <boden/widget/view.hpp>

#include <memory>

namespace boden {
namespace widget {
namespace layout {

template<typename T>
class anchor_t
{
public:
    anchor_t(boden::widget::view_ref_t view, boden::widget::layout::attribute_t attr)
    {
        _view = view;
        _attr = attr;
    }

    virtual ~anchor_t()
    {
    }

    boden::widget::layout::constraint_ref_t constraint_equal_to_anchor(const std::shared_ptr<anchor_t<T>> &anchor,
                                                                       float constant = 0.0f)
    {
        return std::make_shared<constraint_t>(_view.lock(),
                                              _attr,
                                              boden::widget::layout::relation_t::equal,
                                              anchor->get_view(),
                                              anchor->get_attrubute(),
                                              1.0f,
                                              constant);
    }

    boden::widget::layout::attribute_t get_attrubute() const
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

    boden::widget::layout::attribute_t _attr;
    boden::widget::view_wref_t _view;

private:
};

} // layout
} // widget
} // boden
