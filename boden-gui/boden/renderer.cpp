#include "renderer.hpp"

namespace boden {

renderer_t::renderer_t()
{
}

renderer_t::~renderer_t()
{
}

void renderer_t::begin_draw(boden::context_t &ctx)
{
    builder.reset();
    builder.push_clip_rect({0, 0, ctx.display_size.width, ctx.display_size.height});
}

void renderer_t::end_draw(boden::context_t &ctx)
{
}

} // boden
