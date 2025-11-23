#include "webgl_texture_manager.hpp"

namespace platform {
    
extern "C" {

extern uint32_t em_bake(uint32_t gtid,
                        float_t x, 
                        float_t y,
                        float_t width,
                        float_t height,
                        const uint8_t *bytes,
                        uint32_t length,
                        uint32_t number_of_channels);

extern uint32_t em_create_gpu_texture(float_t width, float_t height);
extern void em_destroy_gpu_texture(uint32_t handle);

} // "C"

webgl_texture_manager_t::webgl_texture_manager_t()
    : boden::graphic::texture_manager_t()
{
}

webgl_texture_manager_t::~webgl_texture_manager_t()
{
}

void webgl_texture_manager_t::bake(boden::graphic::gpu_texture_handle_t gpu_texture_handle,
                                   boden::layout::rect_t rect,
                                   const uint8_t *bytes,
                                   size_t length,
                                   uint8_t number_of_channels)
{
    em_bake(gpu_texture_handle,
            rect.origin.x,
            rect.origin.y,
            rect.size.width,
            rect.size.height,
            bytes, 
            length,
            number_of_channels);
}
    
boden::graphic::gpu_texture_handle_t webgl_texture_manager_t::create_gpu_texture(boden::layout::size_t size,
                                                                                 uint8_t number_of_channels)
{
    return (uint64_t)em_create_gpu_texture(size.width, size.height);
}

void webgl_texture_manager_t::destroy_gpu_texture(boden::graphic::gpu_texture_handle_t handle)
{
    em_destroy_gpu_texture((uint32_t)handle);
}

} // platform
