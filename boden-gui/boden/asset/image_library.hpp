#pragma once

#include <boden/asset/texture_id.hpp>
#include <boden/widget/base/image.hpp>
#include <cstdint>
#include <string>
#include <unordered_map>

namespace boden {
namespace asset {

class image_library_t
{
public:
    virtual ~image_library_t() = 0;

    virtual bool load_image_from_path(const std::string &name, const std::string &path);
    virtual bool load_image_from_data(const std::string &name, const boden::widget::base::image_t &image);
    virtual boden::asset::texture_id_t get_texture_id(const char* name) = 0;
    
protected:
    std::unordered_map<std::string, boden::asset::texture_id_t> _image_inventory;
};
    
} // asset
} // boden
