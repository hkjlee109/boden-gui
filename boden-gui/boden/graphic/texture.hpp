#pragma once

#include <boden/layout/rect.hpp>
#include <boden/layout/size.hpp>
#include <boden/graphic/texture_id.hpp>
#include <vector>

namespace boden {
namespace graphic {

struct texture_t
{
    texture_t(boden::graphic::texture_id_t _id,  
              boden::layout::size_t _size, 
              uint8_t _number_of_channels);
    ~texture_t();

    boden::graphic::texture_id_t id;
    boden::layout::size_t size;
    uint8_t number_of_channels;
    std::vector<uint8_t> data;
    
    boden::layout::rect_t dirty_rect;
    bool needs_update;

    boden::graphic::gpu_texture_handle_t gpu_texture_handle;

    void alloc_data_if_needed();
};

} // graphic
} // boden
