#include "renderer.hpp"

namespace boden {

renderer_t::renderer_t()
{
}

renderer_t::~renderer_t()
{
}

void renderer_t::render(boden::context_t &ctx)
{
}

void renderer_t::set_texture_manager(boden::gpu::texture_manager_t *manager)
{
    _texture_manager = manager;
}


} // boden
