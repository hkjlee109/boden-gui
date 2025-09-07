#include "rectangle.hpp"

namespace boden {
namespace widget {
namespace shape {

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

void rectangle_t::draw(boden::builder_t &builder)
{
    if(_hidden) 
    {
        return;
    }

    boden::layout::point_t origin = convert_point_to_view({0, 0}, nullptr);
    boden::layout::rect_t frame{origin, _frame.size};

    builder.push_clip_rect({frame.origin.x - get_layer_border_width() / 2, 
                            frame.origin.y - get_layer_border_width() / 2, 
                            frame.size.width + get_layer_border_width(), 
                            frame.size.height + get_layer_border_width()});
    
    builder.add_rect_filled({frame.origin.x, frame.origin.y}, 
                            {frame.origin.x + frame.size.width, frame.origin.y + frame.size.height},
                            get_layer_background_color());

    builder.add_rect({frame.origin.x, frame.origin.y}, 
                     {frame.origin.x + frame.size.width, frame.origin.y + frame.size.height},
                     get_layer_border_color(),
                     get_layer_border_width());
    
    builder.pop_clip_rect();

    shape_t::draw(builder);
}

void rectangle_t::mouse_down(const boden::event_t &ev)
{
    send_actions(boden::widget::control_event_t::mouse_down);
}

void rectangle_t::mouse_dragged(const boden::event_t &ev)
{
}

void rectangle_t::mouse_up(const boden::event_t &ev)
{
    send_actions(boden::widget::control_event_t::mouse_up);
}

} // shape
} // widget
} // boden
