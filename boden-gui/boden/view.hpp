#pragma once

#include <boden/context.hpp>
#include <boden/responder.hpp>
#include <boden/layer.hpp>
#include <boden/layout/rect.hpp>
#include <vector>

namespace boden {

class view_t : public responder_t, public std::enable_shared_from_this<boden::view_t> 
{
public:
    view_t();
    view_t(const boden::layout::rect_t &frame);
    virtual ~view_t();

    virtual void draw(boden::context_t &ctx);
    virtual std::shared_ptr<boden::view_t> hit_test(boden::layout::point_t point);

    void add_subview(const std::shared_ptr<boden::view_t> &view);

    float get_layer_border_width() const;
    const boden::layout::color_t & get_layer_border_color() const;
    const std::vector<std::shared_ptr<boden::view_t>> & get_subviews() const;

    void set_layer_border_width(float width);

protected:
    boden::layer_t _layer;
    boden::layout::rect_t _bounds;
    boden::layout::rect_t _frame;

    std::vector<std::shared_ptr<boden::view_t>> _subviews;
};

} // boden
