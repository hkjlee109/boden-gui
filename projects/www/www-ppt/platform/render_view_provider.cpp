#include "render_view_provider.hpp"

#include <emscripten.h>

namespace platform {

extern "C" {

extern void em_display_text_input(float x,
                                  float y,
                                  float width,
                                  float height);

// extern void em_render(const boden::draw::command_t *commands,
//                       uint32_t commands_count,
//                       const boden::draw::index_t *indices,
//                       uint32_t indices_count,
//                       const boden::draw::vertex_t *vertices,
//                       uint32_t vertices_count);

// EMSCRIPTEN_KEEPALIVE
// uint32_t cpp_get_gpu_texture_id(boden::gpu::texture_id_t tid) 
// {
//     return (uint32_t)texture_manager->get_gpu_texture_handle(tid);
// }

} // "C"

render_view_provider_t::render_view_provider_t()
{
}

render_view_provider_t::~render_view_provider_t()
{
}

void render_view_provider_t::display_text_input(const boden::layout::rect_t &rect)
{
    em_display_text_input(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
}

} // platform
