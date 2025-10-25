#include "texture_manager.hpp"

namespace boden {
namespace graphic {

texture_t::texture_t(boden::graphic::texture_id_t _id,  
                     boden::layout::size_t _size,
                     uint8_t _number_of_channels)
    : id{_id},
      size{_size},
      number_of_channels{_number_of_channels},
      needs_update{false},
      gpu_texture_handle{0}
{
}

texture_t::~texture_t()
{
}

void texture_t::alloc_data_if_needed()
{
    if(data.size() == 0)
    {
        data.resize(size.width * size.height * number_of_channels);
    }
}

} // graphic
} // boden
