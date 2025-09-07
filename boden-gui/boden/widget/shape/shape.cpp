#include "shape.hpp"

namespace boden {
namespace widget {
namespace shape {

shape_t::shape_t()
{
}

shape_t::shape_t(const boden::layout::rect_t &frame)
    : control_t{frame},
      _selected{false}
{
}

shape_t::~shape_t()
{
}

void shape_t::draw(boden::builder_t &builder) 
{
    if(_selected) 
    {
        boden::layout::point_t origin = convert_point_to_view({0, 0}, nullptr);
        boden::layout::rect_t frame{origin, _frame.size};

        builder.push_clip_rect({frame.origin.x - HANDLE_SIZE_HALF - 1, 
                                frame.origin.y - HANDLE_SIZE_HALF - 1, 
                                frame.size.width + HANDLE_SIZE + 2, 
                                frame.size.height + HANDLE_SIZE + 2});
    
        boden::layout::point_t pts[] = {
            {frame.origin.x, frame.origin.y},
            {frame.origin.x + frame.size.width, frame.origin.y},
            {frame.origin.x, frame.origin.y + frame.size.height},
            {frame.origin.x + frame.size.width, frame.origin.y + frame.size.height},

            {frame.origin.x + frame.size.width / 2, frame.origin.y},
            {frame.origin.x, frame.origin.y + frame.size.height / 2},
            {frame.origin.x + frame.size.width, frame.origin.y + frame.size.height / 2},
            {frame.origin.x + frame.size.width / 2, frame.origin.y + frame.size.height}
        };

        for (const auto &pt : pts) {
            builder.add_rect_filled({pt.x - HANDLE_SIZE_HALF, pt.y - HANDLE_SIZE_HALF}, 
                                    {pt.x + HANDLE_SIZE_HALF, pt.y + HANDLE_SIZE_HALF},
                                    {0xFF, 0xFF, 0xFF, 0xFF});
            builder.add_rect({pt.x - HANDLE_SIZE_HALF, pt.y - HANDLE_SIZE_HALF}, 
                             {pt.x + HANDLE_SIZE_HALF, pt.y + HANDLE_SIZE_HALF},
                             {0x56, 0x56, 0x56, 0xFF},
                             1);
        }

        builder.pop_clip_rect();
    }
}

bool shape_t::is_selected() const
{
    return _selected;
}

void shape_t::set_selected(bool selected)
{
    _selected = selected;
}

} // shape
} // widget
} // boden
