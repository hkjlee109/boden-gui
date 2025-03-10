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

    void set_layer_border_width(float width);

    const boden::layout::color_t & get_layer_border_color() const;
    float get_layer_border_width() const;

protected:
    boden::layer_t _layer;
    boden::layout::rect_t _bounds;
    boden::layout::rect_t _frame;

    std::vector<std::shared_ptr<boden::view_t>> _subviews;
};

} // boden
