#include "webgl_renderer.hpp"

namespace platform {
    
extern "C" {

extern void lib_render(const boden::draw::command_t *commands,
                       uint32_t commands_count,
                       const boden::draw::index_t *indices,
                       uint32_t indices_count,
                       const boden::draw::vertex_t *vertices,
                       uint32_t vertices_count);

} // "C"

webgl_renderer_t::webgl_renderer_t()
{

}

webgl_renderer_t::~webgl_renderer_t()
{

}

void webgl_renderer_t::render(boden::context_t &ctx)
{
    lib_render(ctx.batch->commands.data(),
               ctx.batch->commands.size(),
               ctx.batch->indices.data(),
               ctx.batch->indices.size(),
               ctx.batch->vertices.data(),
               ctx.batch->vertices.size());
}

} // platform
