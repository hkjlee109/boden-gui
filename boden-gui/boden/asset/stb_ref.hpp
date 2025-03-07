#pragma once

namespace boden {
namespace asset {

struct stb_ref_t 
{
    unsigned char *data = nullptr;
    int width = 0;
    int height = 0;
    int number_of_channels = 0;

    stb_ref_t(const char* full_path);
    ~stb_ref_t();

    operator unsigned char * () const {
        return data;
    }
};
    
} // asset
} // boden
