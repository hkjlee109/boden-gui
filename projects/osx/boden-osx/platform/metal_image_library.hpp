#pragma once

#include <boden/asset/image_library.hpp>
#include <boden/asset/texture_id.hpp>
#include <Metal/Metal.hpp>

namespace platform {

class metal_image_library_t : public boden::asset::image_library_t
{
public:
    metal_image_library_t(MTL::Device* device);
    ~metal_image_library_t();
    
    bool load_image(const char *key, const char *full_path) override;
    boden::asset::texture_id_t get_image(const char *name) override;

private:
    MTL::Device *_device;
};

} // platform
