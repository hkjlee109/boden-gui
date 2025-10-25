#pragma once

#include <boden/layout/size.hpp>
#include <boden/graphic/gpu_texture_handle.hpp>
#include <boden/graphic/texture.hpp>
#include <boden/graphic/texture_id.hpp>
#include <unordered_map>

namespace boden {
namespace graphic {

class texture_manager_t
{
public:
    texture_manager_t();
    virtual ~texture_manager_t();

    virtual void bake(boden::graphic::gpu_texture_handle_t gpu_texture_handle,
                      boden::layout::rect_t rect,
                      const uint8_t *bytes,
                      size_t length,
                      uint8_t number_of_channels) = 0;

    virtual boden::graphic::gpu_texture_handle_t create_gpu_texture(boden::layout::size_t size, 
                                                                uint8_t number_of_channels) = 0;
    virtual void destroy_gpu_texture(boden::graphic::gpu_texture_handle_t handle) = 0;

    boden::graphic::gpu_texture_handle_t get_gpu_texture_handle(boden::graphic::texture_id_t tid);
    boden::layout::size_t get_texture_size(boden::graphic::texture_id_t tid) const;

    boden::graphic::texture_id_t create(boden::layout::size_t size,
                                    uint8_t number_of_channels);
    void destroy(texture_id_t tid);

    bool load(boden::graphic::texture_id_t tid, 
              boden::layout::rect_t rect,
              const uint8_t *bytes, 
              size_t length);

    void cleanup_unused_texture();

protected:
    std::unordered_map<boden::graphic::texture_id_t, 
                       std::unique_ptr<boden::graphic::texture_t>> _textures;
    std::vector<std::unique_ptr<boden::graphic::texture_t>> _unused_textures;
    
private:
    boden::graphic::texture_id_t _last_texture_id;
};

} // graphic
} // boden
