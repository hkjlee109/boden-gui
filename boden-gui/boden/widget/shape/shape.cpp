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
        builder.push_clip_rect({_frame.origin.x - HANDLE_SIZE_HALF - 1, 
                                _frame.origin.y - HANDLE_SIZE_HALF - 1, 
                                _frame.size.width + HANDLE_SIZE + 2, 
                                _frame.size.height + HANDLE_SIZE + 2});
    
        boden::layout::point_t pts[] = {
            {_frame.origin.x, _frame.origin.y},
            {_frame.origin.x + _frame.size.width, _frame.origin.y},
            {_frame.origin.x, _frame.origin.y + _frame.size.height},
            {_frame.origin.x + _frame.size.width, _frame.origin.y + _frame.size.height},

            {_frame.origin.x + _frame.size.width / 2, _frame.origin.y},
            {_frame.origin.x, _frame.origin.y + _frame.size.height / 2},
            {_frame.origin.x + _frame.size.width, _frame.origin.y + _frame.size.height / 2},
            {_frame.origin.x + _frame.size.width / 2, _frame.origin.y + _frame.size.height}
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
