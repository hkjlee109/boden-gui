#pragma once

#include <boden/layout/rect.hpp>
#include <boden/widget/base/image.hpp>
#include <boden/widget/layer/layer.hpp>
#include <memory>

namespace boden {
namespace widget {
namespace layer {

class image_layer_t : public boden::widget::layer::layer_t
{
public:
    static std::shared_ptr<image_layer_t> alloc();
    static std::shared_ptr<image_layer_t> alloc(const boden::layout::rect_t &frame);

    void draw(boden::builder_t &builder, const boden::layout::rect_t &parent_frame_in_window) override;
    
    image_layer_t();
    explicit image_layer_t(const boden::layout::rect_t &frame);
    ~image_layer_t();
    
    void set_image(std::unique_ptr<boden::widget::base::image_t> image);

    const boden::layout::color_t & get_tint_color() const;
    void set_tint_color(const boden::layout::color_t &color);

protected:
    void init() override;
    void init(const boden::layout::rect_t &frame) override;

private:
    std::unique_ptr<boden::widget::base::image_t> _image;
    boden::layout::color_t _tint_color;
};

} // layer
} // widget
} // boden
