#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

namespace boden {
namespace asset {

class image_library_t
{
public:
    virtual ~image_library_t() = 0;
    
    virtual bool load_image(const char* key, const char* full_path) = 0;
    virtual uint64_t get_image(const char* key) = 0;

protected:
    std::unordered_map<std::string, uint64_t> _image_inventory;
};
    
} // asset
} // boden