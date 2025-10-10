#include "textbox.hpp"

#include <any>
#include <boden/system_event.hpp>

namespace ppt {
namespace widget {
namespace shape {

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

} // shape
} // widget
} // ppt
