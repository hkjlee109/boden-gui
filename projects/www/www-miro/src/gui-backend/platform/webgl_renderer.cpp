#include "webgl_renderer.hpp"

#include <emscripten.h>

namespace platform {

boden::gpu::texture_manager_t *texture_manager;

extern "C" {

extern void em_setup();

extern void em_render(const boden::draw::command_t *commands,
                      uint32_t commands_count,
                      const boden::draw::index_t *indices,
                      uint32_t indices_count,
                      const boden::draw::vertex_t *vertices,
                      uint32_t vertices_count);

EMSCRIPTEN_KEEPALIVE
uint32_t cpp_get_gpu_texture_id(boden::gpu::texture_id_t tid) 
{
    return (uint32_t)texture_manager->get_gpu_texture_handle(tid);
}

} // "C"

webgl_renderer_t::webgl_renderer_t()
    : boden::renderer_t()
{
}

webgl_renderer_t::~webgl_renderer_t()
{
}

void webgl_renderer_t::set_texture_manager(boden::gpu::texture_manager_t *manager)
{
    boden::renderer_t::set_texture_manager(manager);
    texture_manager = _texture_manager;
    em_setup();
}

void webgl_renderer_t::render(boden::context_t &ctx)
{
    em_render(ctx.batch->commands.data(),
              ctx.batch->commands.size(),
              ctx.batch->indices.data(),
              ctx.batch->indices.size(),
              ctx.batch->vertices.data(),
              ctx.batch->vertices.size());
}

} // platform
