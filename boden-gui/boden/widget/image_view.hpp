#pragma once

#include <boden/builder.hpp>
#include <boden/layout/rect.hpp>
#include <boden/widget/base/image.hpp>
#include <boden/widget/view.hpp>

namespace boden {
namespace widget {

class image_view_t : public boden::widget::view_t
{
public:
    image_view_t();
    image_view_t(const boden::layout::rect_t &frame);
    virtual ~image_view_t();
    
    virtual void draw(boden::builder_t &builder) override;

    void set_image(std::unique_ptr<boden::widget::base::image_t> image);
    void set_tint_color(const boden::layout::color_t &color);

    const boden::layout::color_t & get_tint_color() const;

private:
    std::unique_ptr<boden::widget::base::image_t> _image;
    boden::layout::color_t _tint_color;
};

} // widget
} // boden
