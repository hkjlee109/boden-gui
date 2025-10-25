#pragma once

#include <boden/gpu/texture_manager.hpp>

#include <Metal/Metal.hpp>

namespace platform {

class mtl_texture_manager_t : public boden::gpu::texture_manager_t
{
public:
    mtl_texture_manager_t(MTL::Device *device);
    ~mtl_texture_manager_t() override;
    
    void bake(boden::gpu::gpu_texture_handle_t gpu_texture_handle,
              boden::layout::rect_t rect,
              const uint8_t *bytes,
              size_t length,
              uint8_t number_of_channels) override;
    
    boden::gpu::gpu_texture_handle_t create_gpu_texture(boden::layout::size_t size, 
                                                        uint8_t number_of_channels) override;
    void destroy_gpu_texture(boden::gpu::gpu_texture_handle_t handle) override;
    
private:
    MTL::Device *_device;
};

} // platform
