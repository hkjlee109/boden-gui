#include "button.hpp"
#include <boden/renderer.hpp>

namespace boden {

button_t::button_t()
    : view_t(),
      bounds_{0, 0, 0, 0},
      frame_{0, 0, 0, 0}
{
}

button_t::~button_t()
{
}

button_t::button_t(boden::geometry::rect_t frame)
    : view_t(),
      bounds_{0, 0, frame.size.width, frame.size.height},
      frame_{frame}
{
}

void button_t::draw(boden::context_t &ctx)
{
    ctx.renderer->builder.add_rect({frame_.origin.x, frame_.origin.y}, 
                                   {frame_.origin.x + frame_.size.width, frame_.origin.y + frame_.size.height});
}

} // boden
