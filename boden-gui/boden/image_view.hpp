#pragma once

#include <boden/builder.hpp>
#include <boden/image.hpp>
#include <boden/layout/rect.hpp>
#include <boden/view.hpp>

namespace boden {

class image_view_t : public view_t
{
public:
    image_view_t();
    image_view_t(const boden::layout::rect_t &frame);
    virtual ~image_view_t();
    
    virtual void draw(boden::builder_t &builder) override;

    void set_image(std::unique_ptr<boden::image_t> image);
    void set_tint_color(const boden::layout::color_t &color);

    const boden::layout::color_t & get_tint_color() const;

private:
    std::unique_ptr<boden::image_t> _image;
    boden::layout::color_t _tint_color;
};

} // boden
