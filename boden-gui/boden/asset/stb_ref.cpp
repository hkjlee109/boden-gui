#include "stb_ref.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <deps/stb/stb_image.h>

namespace boden {
namespace asset {

stb_ref_t::stb_ref_t(const char* full_path)
{    
    data = stbi_load(full_path, &width, &height, &number_of_channels, 4);

    if(data) 
    {
        uint32_t size = width * height;
        for(uint32_t i = 0; i < size; ++i) 
        {
            float a = data[i * 4 + 3] / 255.0f;
            data[i * 4 + 0] = static_cast<unsigned char>(data[i * 4 + 0] * a);
            data[i * 4 + 1] = static_cast<unsigned char>(data[i * 4 + 1] * a);
            data[i * 4 + 2] = static_cast<unsigned char>(data[i * 4 + 2] * a);
        }
    }
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
