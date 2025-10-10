#include "texture_manager.hpp"

namespace boden {
namespace gpu {

texture_t::texture_t(boden::gpu::texture_id_t _id,  
                     boden::layout::size_t _size,
                     uint8_t _number_of_channels)
    : id{_id},
      size{_size},
      number_of_channels{_number_of_channels},
      needs_update{false},
      gpu_texture_handle{0}
{
    data.resize(size.height * size.width * number_of_channels);
}

texture_t::~texture_t()
{
}

} // gpu
} // boden
