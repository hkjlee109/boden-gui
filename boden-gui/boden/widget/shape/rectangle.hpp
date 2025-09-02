#pragma once

#include <boden/layout/rect.hpp>
#include <boden/widget/control.hpp>

namespace boden {
namespace widget {
namespace shape {

class rectangle_t : public boden::widget::control_t
{
public:
    rectangle_t();
    rectangle_t(const boden::layout::rect_t &frame);
    virtual ~rectangle_t();

    virtual void draw(boden::builder_t &builder) override;
    virtual void mouse_down(const boden::event_t &ev) override;
    virtual void mouse_dragged(const boden::event_t &ev) override;
    virtual void mouse_up(const boden::event_t &ev) override;

private:
};

} // shape
} // widget
} // boden
