#pragma once

#include <boden/context.hpp>
#include <boden/layer.hpp>
#include <boden/layout/rect.hpp>
#include <vector>

namespace boden {

class view_t
{
public:
    view_t();
    view_t(const boden::layout::rect_t &frame);
    virtual ~view_t();

    virtual void draw(boden::context_t &ctx) = 0;

    boden::layer_t layer;
    std::vector<std::shared_ptr<boden::view_t>> subviews;

protected:
    boden::layout::rect_t _bounds;
    boden::layout::rect_t _frame;
};

} // boden
