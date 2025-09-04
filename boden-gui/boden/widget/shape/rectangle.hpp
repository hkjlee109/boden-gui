#pragma once

#include <boden/layout/rect.hpp>
#include <boden/widget/shape/shape.hpp>

namespace boden {
namespace widget {
namespace shape {

class rectangle_t : public boden::widget::shape::shape_t
{
public:
    rectangle_t();
    rectangle_t(const boden::layout::rect_t &frame);
    ~rectangle_t() override;

    void draw(boden::builder_t &builder) override;
    void mouse_down(const boden::event_t &ev) override;
    void mouse_dragged(const boden::event_t &ev) override;
    void mouse_up(const boden::event_t &ev) override;

private:
};

} // shape
} // widget
} // boden
