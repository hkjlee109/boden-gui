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
}

void renderer_t::end_draw(boden::context_t &ctx)
{
}

} // boden
