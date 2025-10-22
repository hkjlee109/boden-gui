#pragma once

#include <boden/gpu/texture_manager.hpp>

namespace platform {

class webgl_texture_manager_t : public boden::gpu::texture_manager_t
{
public:
    webgl_texture_manager_t();
    ~webgl_texture_manager_t() override;

    void bake(boden::gpu::gpu_texture_handle_t gpu_texture_handle,
              boden::layout::rect_t rect,
              const uint8_t *bytes,
              size_t length,
              uint8_t number_of_channels) override;
    
    boden::gpu::gpu_texture_handle_t create_gpu_texture(boden::layout::size_t size, 
                                                        uint8_t number_of_channels) override;
};

} // platform
