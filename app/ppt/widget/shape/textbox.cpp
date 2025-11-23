#include "textbox.hpp"

#include <boden/system_event.hpp>

namespace ppt {
namespace widget {
namespace shape {

std::shared_ptr<textbox_t> textbox_t::alloc(const boden::layout::rect_t &frame)
{
    auto instance = std::make_shared<textbox_t>(frame);
    instance->init(frame);
    return instance;
}

textbox_t::textbox_t()
    : ppt::widget::shape::shape_t()
{
}

textbox_t::textbox_t(const boden::layout::rect_t &frame)
    : ppt::widget::shape::shape_t(frame)
{
}

textbox_t::~textbox_t()
{
}

void textbox_t::init(const boden::layout::rect_t &frame)
{
    ppt::widget::shape::shape_t::init(frame);
}

} // shape
} // widget
} // ppt
