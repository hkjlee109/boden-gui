#include "rectangle.hpp"

namespace boden {
namespace widget {
namespace shape {

rectangle_t::rectangle_t()
{
}

rectangle_t::rectangle_t(const boden::layout::rect_t &frame)
    : control_t{frame}
{
}

rectangle_t::~rectangle_t()
{
}

void rectangle_t::draw(boden::builder_t &builder)
{
    if(_hidden) 
    {
        return;
    }

    builder.push_clip_rect({_frame.origin.x - get_layer_border_width() / 2, 
                            _frame.origin.y - get_layer_border_width() / 2, 
                            _frame.size.width + get_layer_border_width(), 
                            _frame.size.height + get_layer_border_width()});
    
    builder.add_rect_filled({_frame.origin.x, _frame.origin.y}, 
                            {_frame.origin.x + _frame.size.width, _frame.origin.y + _frame.size.height},
                            get_layer_background_color());

    builder.add_rect({_frame.origin.x, _frame.origin.y}, 
                     {_frame.origin.x + _frame.size.width, _frame.origin.y + _frame.size.height},
                     get_layer_border_color(),
                     get_layer_border_width());
    
    builder.pop_clip_rect();
}

void rectangle_t::mouse_down(const boden::event_t &ev)
{
}

void rectangle_t::mouse_dragged(const boden::event_t &ev)
{
}

void rectangle_t::mouse_up(const boden::event_t &ev)
{
}

} // shape
} // widget
} // boden
