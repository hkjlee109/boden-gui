#pragma once

#include <boden/layout/size.hpp>
#include <boden/gpu/gpu_texture_handle.hpp>
#include <boden/gpu/texture.hpp>
#include <boden/gpu/texture_id.hpp>
#include <unordered_map>

namespace boden {
namespace gpu {

class texture_manager_t
{
public:
    texture_manager_t();
    virtual ~texture_manager_t();

    virtual void bake(boden::gpu::gpu_texture_handle_t gpu_texture_handle,
                      boden::layout::rect_t rect,
                      const uint8_t *bytes,
                      size_t length,
                      uint8_t number_of_channels) = 0;

    virtual boden::gpu::gpu_texture_handle_t create_gpu_texture(boden::layout::size_t size, uint8_t number_of_channels) = 0;

    boden::gpu::gpu_texture_handle_t get_gpu_texture_handle(boden::gpu::texture_id_t tid);
    boden::layout::size_t get_texture_size(boden::gpu::texture_id_t tid) const;

    boden::gpu::texture_id_t create(boden::layout::size_t size,
                                    uint8_t number_of_channels);

    bool load(boden::gpu::texture_id_t tid, 
              boden::layout::rect_t rect,
              const uint8_t *bytes, 
              size_t length);

protected:
    std::unordered_map<boden::gpu::texture_id_t, 
                       std::unique_ptr<boden::gpu::texture_t>> _textures;
    
private:
    boden::gpu::texture_id_t _last_texture_id;
};

} // gpu
} // boden
