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

    float padding = layer.border_width + 1;
    builder.push_clip_rect({frame.origin.x - padding, 
                            frame.origin.y - padding, 
                            frame.size.width + padding * 2, 
                            frame.size.height + padding * 2});
    
    builder.add_rect_filled({frame.origin.x, frame.origin.y}, 
                            {frame.origin.x + frame.size.width, frame.origin.y + frame.size.height},
                            layer.background_color);

    builder.add_rect({frame.origin.x, frame.origin.y}, 
                     {frame.origin.x + frame.size.width, frame.origin.y + frame.size.height},
                     layer.border_color,
                     layer.border_width);
    
    builder.pop_clip_rect();

    shape_t::draw(builder);
}

} // shape
} // widget
} // boden
