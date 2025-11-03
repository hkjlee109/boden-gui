#pragma once

#include <boden/layout/rect.hpp>
#include <ppt/widget/shape/shape.hpp>

namespace ppt {
namespace widget {
namespace shape {

class rectangle_t : public ppt::widget::shape::shape_t
{
public:
    static std::shared_ptr<rectangle_t> alloc(const boden::layout::rect_t &frame);

    rectangle_t();
    explicit rectangle_t(const boden::layout::rect_t &frame);
    ~rectangle_t() override;
    
    void draw_rect(boden::builder_t &builder, const boden::layout::rect_t &dirty_rect) override;

protected:
    void init(const boden::layout::rect_t &frame) override;
};

} // shape
} // widget
} // ppt
