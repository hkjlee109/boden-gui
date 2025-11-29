#pragma once

#include <vector>

namespace boden {
namespace asset {

struct nanosvg_ref_t 
{
    int width = 0;
    int height = 0;
    int number_of_channels = 0;

    nanosvg_ref_t(const char *full_path, uint32_t scale);
    ~nanosvg_ref_t();

    operator const unsigned char * () const {
        return _data.data();
    }

private:
    std::vector<unsigned char> _data;

};
    
} // asset
} // boden
