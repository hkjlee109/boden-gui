#pragma once

#include <boden/builder.hpp>
#include <boden/graphic/texture_id.hpp>
#include <boden/layout/color.hpp>

namespace boden {
namespace widget {
namespace layer {

class layer_t
{
public:
    static std::shared_ptr<layer_t> alloc();
    static std::shared_ptr<layer_t> alloc(const boden::layout::rect_t &frame);

    layer_t();
    explicit layer_t(const boden::layout::rect_t &frame);
    ~layer_t();

    virtual void draw(boden::builder_t &builder, const boden::layout::rect_t &parent_frame_in_window);

    const boden::layout::color_t & get_background_color() const;
    void set_background_color(const boden::layout::color_t &color);

    const boden::layout::color_t & get_border_color() const;
    void set_border_color(const boden::layout::color_t &color);

    float get_border_width() const;
    void set_border_width(float width);

    float get_contents_scale() const;
    void set_contents_scale(float scale);

    float get_corner_radius() const;
    void set_corner_radius(float radius);

    const boden::layout::rect_t & get_frame() const;
    void set_frame(const boden::layout::rect_t &frame);

    bool get_needs_display() const;
    void set_needs_display(bool needs);

    const std::vector<std::shared_ptr<boden::widget::layer::layer_t>> & get_sublayers() const;

    boden::graphic::texture_id_t get_texture_id() const;
    void set_texture_id(boden::graphic::texture_id_t tid);

    void add_layer(std::shared_ptr<boden::widget::layer::layer_t> layer);
    void remove_layer(std::shared_ptr<boden::widget::layer::layer_t> layer);

protected:
    boden::layout::rect_t _bounds;
    boden::layout::rect_t _frame;
    boden::layout::color_t _background_color;
    boden::layout::color_t _border_color;
    float _border_width;
    float _contents_scale;
    float _corner_radius;
    bool _needs_display;
    boden::graphic::texture_id_t _tid;

    std::vector<std::shared_ptr<boden::widget::layer::layer_t>> _sublayers;

    virtual void init();
    virtual void init(const boden::layout::rect_t &frame);
};

} // layer
} // widget
} // boden
