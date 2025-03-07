#include "stb_ref.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace boden {
namespace asset {

stb_ref_t::stb_ref_t(const char* full_path)
{    
    data = stbi_load(full_path, &width, &height, &number_of_channels, 0);
}

stb_ref_t::~stb_ref_t()
{
    if(data)
    {
        stbi_image_free(data);
    }
}

} // asset
} // boden
