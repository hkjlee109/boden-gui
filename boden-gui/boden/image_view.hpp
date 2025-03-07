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
    ~image_view_t();
    
    image_view_t(boden::layout::rect_t frame);

    virtual void draw(boden::context_t &context) override;

    std::unique_ptr<boden::image_t> image;

private:
    boden::layout::rect_t _bounds;
    boden::layout::rect_t _frame;
};

} // boden
