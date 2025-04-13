#include "webgl_renderer.hpp"

namespace platform {
    
extern "C" {

extern void lib_begin_draw();
extern void lib_end_draw(boden::draw::command_t *commands,
                         uint32_t commands_count,
                         boden::draw::index_t *indices,
                         uint32_t indices_count,
                         boden::draw::vertex_t *vertices,
                         uint32_t vertices_count);

} // "C"

webgl_renderer_t::webgl_renderer_t()
{

}

webgl_renderer_t::~webgl_renderer_t()
{

}

void webgl_renderer_t::begin_draw(boden::context_t &ctx)
{
    lib_begin_draw();
}

void webgl_renderer_t::end_draw(boden::context_t &ctx)
{
    lib_end_draw(builder.commands.data(),
                 builder.commands.size(),
                 builder.indices.data(),
                 builder.indices.size(),
                 builder.vertices.data(),
                 builder.vertices.size());
}

} // platform
