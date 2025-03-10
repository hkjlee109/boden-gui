#include "button.hpp"
#include <boden/renderer.hpp>

namespace boden {

button_t::button_t()
    : view_t()
{
}

button_t::button_t(const boden::layout::rect_t &frame)
    : view_t{frame}
{
}

button_t::~button_t()
{
}

void button_t::draw(boden::context_t &ctx)
{
    ctx.renderer->builder.push_clip_rect({_frame.origin.x - layer.border_width / 2, 
                                          _frame.origin.y - layer.border_width / 2, 
                                          _frame.size.width + layer.border_width, 
                                          _frame.size.height + layer.border_width});
    
    ctx.renderer->builder.add_rect({_frame.origin.x, _frame.origin.y}, 
                                   {_frame.origin.x + _frame.size.width, _frame.origin.y + _frame.size.height},
                                   layer.border_color,
                                   layer.border_width);
    
    ctx.renderer->builder.pop_clip_rect();
}

} // boden
