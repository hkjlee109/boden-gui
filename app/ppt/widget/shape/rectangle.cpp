#include "rectangle.hpp"

namespace ppt {
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

void rectangle_t::draw_rect(boden::builder_t &builder, const boden::layout::rect_t &dirty_rect)
{
    if(_hidden) 
    {
        return;
    }

    if(layer.tid == 0)
    {
        return;
    }
    
    boden::layout::rect_t frame_in_window = convert_rect_to_view(_bounds, nullptr);

    builder.begin(layer.tid, frame_in_window, dirty_rect);
    
    boden::layout::rect_t bounds = _bounds.inset_by(1, 1);
    builder.add_rect_filled({bounds.origin.x, bounds.origin.y},
                            {bounds.origin.x + bounds.size.width, bounds.origin.y + bounds.size.height},
                            layer.background_color);

    builder.add_rect({bounds.origin.x, bounds.origin.y},
                     {bounds.origin.x + bounds.size.width, bounds.origin.y + bounds.size.height},
                     layer.border_color,
                     layer.border_width);
    
    shape_t::draw_rect(builder, dirty_rect);

    builder.end();
}

} // shape
} // widget
} // ppt
