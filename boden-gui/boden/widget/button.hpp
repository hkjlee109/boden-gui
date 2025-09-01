#pragma once

#include <boden/builder.hpp>
#include <boden/layout/rect.hpp>
#include <boden/widget/control.hpp>

namespace boden {
namespace widget {

class button_t : public boden::widget::control_t
{
public:
    button_t();
    button_t(const boden::layout::rect_t &frame);
    virtual ~button_t();

    virtual void draw(boden::builder_t &builder) override;
    virtual void mouse_down(const boden::event_t &ev) override;

private:
};

} // widget
} // boden
