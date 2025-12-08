#pragma once

#include <boden/widget/layout/layout_constraint.hpp>

#include <memory>

namespace boden {
namespace widget {
namespace layout {

template<typename T>
class layout_anchor_t
{
public:
    layout_anchor_t()
    {
    }

    virtual ~layout_anchor_t()
    {
    }

    std::shared_ptr<layout_constraint_t> constraint_equal_to_anchor(const std::shared_ptr<layout_anchor_t<T>> &anchor)
    {
        return nullptr;
        // return std::make_shared<layout_constraint_t>(_view,
        //                                              _attribute,
        //                                              layout_relation_t::equal,
        //                                              anchor->_view,
        //                                              anchor->_attribute,
        //                                              1.0f,
        //                                              0.0f);
    }

protected:
    

    virtual void init()
    {
    }

private:
};

} // layout
} // widget
} // boden
