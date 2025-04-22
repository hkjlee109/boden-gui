#pragma once

#include <boden/asset/image_library.hpp>
#include <boden/asset/texture_id.hpp>
#include <boden/image.hpp>
#include <Metal/Metal.hpp>
#include <string>
#include <vector>
#include <unordered_map>

namespace platform {

class mtl_image_library_t : public boden::asset::image_library_t
{
public:
    mtl_image_library_t(MTL::Device *device);
    ~mtl_image_library_t();
    
    bool load_image_from_path(const std::string &name, const std::string &path) override;
    bool load_image_from_data(const std::string &name, const boden::image_t &image) override;
    boden::asset::texture_id_t get_texture_id(const char *name) override;

    MTL::Texture * get_mtl_texture(boden::asset::texture_id_t texture_id);
    
private:
    MTL::Device *_device;
    
    uint32_t _last_texture_id;
    std::unordered_map<boden::asset::texture_id_t, MTL::Texture *> _textures;
};

} // platform
