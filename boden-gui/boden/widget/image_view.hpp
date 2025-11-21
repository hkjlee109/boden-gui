#pragma once

#include <boden/builder.hpp>
#include <boden/layout/rect.hpp>
#include <boden/widget/base/image.hpp>
#include <boden/widget/layer/image_layer.hpp>
#include <boden/widget/view.hpp>

namespace boden {
namespace widget {

class image_view_t : public boden::widget::view_t
{
public:
    static std::shared_ptr<image_view_t> alloc(const boden::layout::rect_t &frame);

    image_view_t();
    image_view_t(const boden::layout::rect_t &frame);
    ~image_view_t() override;
    
    void draw_rect(boden::builder_t &builder, const boden::layout::rect_t &dirty_rect) override;
    void view_will_move_to_window(std::shared_ptr<boden::widget::window_t> window) override;
    void set_frame(const boden::layout::rect_t &frame) override;

    void set_image(std::unique_ptr<boden::widget::base::image_t> image);

    const boden::layout::color_t & get_tint_color() const;
    void set_tint_color(const boden::layout::color_t &color);

protected:
    void init(const boden::layout::rect_t &frame) override;

private:
    std::shared_ptr<boden::widget::layer::image_layer_t> _image_layer;
    
    void create_image_layer_texture();
};

} // widget
} // boden
