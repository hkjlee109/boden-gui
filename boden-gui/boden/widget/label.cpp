#include "label.hpp"
#include <boden/renderer.hpp>

namespace boden {
namespace widget {

label_t::label_t()
    : view_t()
{
}

label_t::label_t(const boden::layout::rect_t &frame)
    : view_t{frame}
{
}

label_t::~label_t()
{
}

void label_t::draw(boden::builder_t &builder)
{
    if(_hidden) 
    {
        return;
    }
    
    boden::layout::point_t origin = convert_point_to_view({0, 0}, nullptr);
    boden::layout::rect_t frame{origin, _frame.size};

    builder.push_clip_rect({frame.origin.x, frame.origin.y, frame.size.width, frame.size.height});
    
    builder.pop_clip_rect();
}

} // widget
} // boden
