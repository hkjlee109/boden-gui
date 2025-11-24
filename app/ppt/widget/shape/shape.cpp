#include "shape.hpp"

#include <boden/layout/point.hpp>
#include <ppt/theme/color.hpp>

namespace ppt {
namespace widget {
namespace shape {

std::shared_ptr<shape_t> shape_t::alloc(const boden::layout::rect_t &frame)
{
    auto instance = std::make_shared<shape_t>(frame);
    instance->init(frame);
    return instance;
}

shape_t::shape_t()
    : control_t{}
{
}

shape_t::shape_t(const boden::layout::rect_t &frame)
    : control_t{frame}
{
}

shape_t::~shape_t()
{
}

boden::widget::view_ref_t shape_t::hit_test(boden::layout::point_t point)
{
    if(_hidden)
    {
        return nullptr;
    }

    boden::layout::rect_t frame_in_window = convert_rect_to_view(_bounds, nullptr);
    
    if(!frame_in_window.contains(point))
    {
        return nullptr;
    }

    return shared_from_this();
}

bool shape_t::is_editable() const
{
    return _text_field->is_enabled();
}

void shape_t::set_editable(bool editable)
{
    _text_field->set_enabled(editable);
}

const boden::layout::rect_t & shape_t::get_frame_cache() const
{
    return _frame_cache;
}

void shape_t::set_frame_cache(const boden::layout::rect_t& frame)
{
    _frame_cache = frame;
}

void shape_t::set_text(const std::string &text)
{
    _text_field->set_text(text);
}

void shape_t::set_text_color(const boden::layout::color_t &color)
{
    _text_field->set_text_color(color);
}

void shape_t::init(const boden::layout::rect_t &frame)
{
    boden::widget::view_t::init(frame);
    _text_field = boden::widget::text_field_t::alloc({{0, 0}, frame.size});
    add_subview(_text_field);
}

} // shape
} // widget
} // ppt
