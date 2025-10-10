#pragma once

#include <boden/layout/color.hpp>
#include <boden/layout/rect.hpp>
#include <ppt/widget/shape/shape.hpp>

namespace ppt {
namespace widget {
namespace shape {

class textbox_t : public ppt::widget::shape::shape_t
{
public:
    textbox_t();
    textbox_t(const boden::layout::rect_t &frame);
    ~textbox_t() override;

private:
};

} // shape
} // widget
} // ppt
