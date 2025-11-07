#include "rectangle.hpp"

namespace ppt {
namespace widget {
namespace shape {

std::shared_ptr<rectangle_t> rectangle_t::alloc(const boden::layout::rect_t &frame)
{
    auto instance = std::make_shared<rectangle_t>(frame);
    instance->init(frame);
    return instance;
}

rectangle_t::rectangle_t()
{
}

rectangle_t::rectangle_t(const boden::layout::rect_t &frame)
    : shape_t{frame}
{
}

rectangle_t::~rectangle_t()
{
}

void rectangle_t::draw_rect(boden::builder_t &builder, const boden::layout::rect_t &dirty_rect)
{
    shape_t::draw_rect(builder, dirty_rect);
}

void rectangle_t::init(const boden::layout::rect_t &frame)
{
    ppt::widget::shape::shape_t::init(frame);
}

} // shape
} // widget
} // ppt
