#pragma once

#include <boden/layout/rect.hpp>
#include <ppt/widget/shape/shape.hpp>

namespace ppt {
namespace widget {
namespace shape {

class rectangle_t : public ppt::widget::shape::shape_t
{
public:
    rectangle_t();
    rectangle_t(const boden::layout::rect_t &frame);
    ~rectangle_t() override;

    void draw(boden::builder_t &builder) override;

private:
};

} // shape
} // widget
} // ppt
