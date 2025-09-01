#pragma once

#include <boden/builder.hpp>
#include <boden/widget/base/layer.hpp>
#include <boden/widget/base/responder.hpp>
#include <boden/layout/rect.hpp>
#include <vector>

namespace boden {
namespace widget {

class view_t : public boden::widget::base::responder_t, public std::enable_shared_from_this<boden::widget::view_t> 
{
public:
    view_t();
    view_t(const boden::layout::rect_t &frame);
    virtual ~view_t();

    virtual void draw(boden::builder_t &builder);
    virtual std::shared_ptr<boden::widget::view_t> hit_test(boden::layout::point_t point);

    void add_subview(const std::shared_ptr<boden::widget::view_t> &view);

    float get_layer_border_width() const;
    void set_layer_border_width(float width);

    const boden::layout::color_t & get_layer_border_color() const;
    void set_layer_border_color(const boden::layout::color_t &color);

    const std::vector<std::shared_ptr<boden::widget::view_t>> & get_subviews() const;

    bool is_hidden() const;
    void set_hidden(bool hidden);

protected:
    boden::widget::base::layer_t _layer;
    boden::layout::rect_t _bounds;
    boden::layout::rect_t _frame;
    bool _hidden;

    std::vector<std::shared_ptr<boden::widget::view_t>> _subviews;
};

} // widget
} // boden
