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

    boden::layout::point_t origin = convert_point_to_view({0, 0}, nullptr);
    boden::layout::rect_t frame{origin, _frame.size};
    boden::layout::rect_t clip_rect{origin + dirty_rect.origin, dirty_rect.size};
        
    float padding = layer.border_width + 1;
    builder.push_clip_rect({clip_rect.origin.x - padding, 
                            clip_rect.origin.y - padding, 
                            clip_rect.size.width + padding * 2, 
                            clip_rect.size.height + padding * 2});
    
    builder.add_rect_filled({frame.origin.x, frame.origin.y}, 
                            {frame.origin.x + frame.size.width, frame.origin.y + frame.size.height},
                            layer.background_color);

    builder.add_rect({frame.origin.x, frame.origin.y}, 
                     {frame.origin.x + frame.size.width, frame.origin.y + frame.size.height},
                     layer.border_color,
                     layer.border_width);
    
    builder.pop_clip_rect();

    shape_t::draw_rect(builder, dirty_rect);
}

} // shape
} // widget
} // ppt
