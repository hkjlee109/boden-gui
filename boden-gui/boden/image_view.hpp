#pragma once

#include <boden/context.hpp>
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
    
    virtual void draw(boden::context_t &context) override;

    std::unique_ptr<boden::image_t> image;

private:
    boden::layout::color_t _tint_color;
};

} // boden
