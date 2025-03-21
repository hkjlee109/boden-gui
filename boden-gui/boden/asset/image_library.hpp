#pragma once

#include <boden/asset/texture_id.hpp>
#include <cstdint>
#include <string>
#include <unordered_map>

namespace boden {
namespace asset {

class image_library_t
{
public:
    virtual ~image_library_t() = 0;

    virtual bool load_image(const char* name, const char* full_path) = 0;
    virtual boden::asset::texture_id_t get_image(const char* name) = 0;

protected:
    std::unordered_map<std::string, boden::asset::texture_id_t> _image_inventory;
};
    
} // asset
} // boden