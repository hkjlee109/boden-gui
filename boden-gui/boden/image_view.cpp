#include "image_view.hpp"
#include <boden/renderer.hpp>

namespace boden {

image_view_t::image_view_t()
    : view_t(),
      _tint_color{}
{
}

image_view_t::image_view_t(const boden::layout::rect_t &frame)
    : view_t{frame}
{
}

image_view_t::~image_view_t()
{
}

void image_view_t::draw(boden::context_t &ctx)
{
    ctx.renderer->builder.add_image(image->texture_id, 
                                    {_frame.origin.x, _frame.origin.y}, 
                                    {_frame.origin.x + _frame.size.width, _frame.origin.y + _frame.size.height},
                                    _tint_color);
}

} // boden
