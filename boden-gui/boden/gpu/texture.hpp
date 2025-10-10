#pragma once

#include <boden/layout/rect.hpp>
#include <boden/layout/size.hpp>
#include <boden/gpu/texture_id.hpp>
#include <vector>

namespace boden {
namespace gpu {

struct texture_t
{
    texture_t(boden::gpu::texture_id_t _id,  
              boden::layout::size_t _size, 
              uint8_t _number_of_channels);
    ~texture_t();

    boden::gpu::texture_id_t id;
    boden::layout::size_t size;
    uint8_t number_of_channels;
    std::vector<uint8_t> data;
    
    boden::layout::rect_t dirty_rect;
    bool needs_update;

    boden::gpu::gpu_texture_handle_t gpu_texture_handle;
};

} // gpu
} // boden
