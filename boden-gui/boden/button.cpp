#include "button.hpp"
#include <boden/renderer.hpp>

namespace boden {

button_t::button_t()
    : control_t{}
{
}

button_t::button_t(const boden::layout::rect_t &frame)
    : control_t{frame}
{
}

button_t::~button_t()
{
}

void button_t::draw(boden::context_t &ctx)
{
    ctx.renderer->builder.push_clip_rect({_frame.origin.x - get_layer_border_width() / 2, 
                                          _frame.origin.y - get_layer_border_width() / 2, 
                                          _frame.size.width + get_layer_border_width(), 
                                          _frame.size.height + get_layer_border_width()});
    
    ctx.renderer->builder.add_rect({_frame.origin.x, _frame.origin.y}, 
                                   {_frame.origin.x + _frame.size.width, _frame.origin.y + _frame.size.height},
                                   get_layer_border_color(),
                                   get_layer_border_width());
    
    ctx.renderer->builder.pop_clip_rect();
}


void button_t::mouse_down(const boden::event_t &ev)
{
    printf("HI\n");
}

} // boden
