#pragma once

#include <boden/asset/image_library.hpp>
#include <boden/asset/texture_id.hpp>
#include <boden/image.hpp>
#include <Metal/Metal.hpp>
#include <string>
#include <vector>

namespace platform {

class metal_image_library_t : public boden::asset::image_library_t
{
public:
    metal_image_library_t(MTL::Device* device);
    ~metal_image_library_t();
    
    bool load_image_from_path(const std::string &name, const std::string &path) override;
    bool load_image_from_data(const std::string &name, const boden::image_t &image) override;
    boden::asset::texture_id_t get_image(const char *name) override;

private:
    MTL::Device *_device;
};

} // platform
