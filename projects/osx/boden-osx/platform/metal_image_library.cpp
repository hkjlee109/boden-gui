#include "metal_image_library.hpp"
#include <boden/asset/stb_ref.hpp>

namespace platform {

metal_image_library_t::metal_image_library_t(MTL::Device *device)
    : boden::asset::image_library_t(),
      _device{device}
{
}

metal_image_library_t::~metal_image_library_t()
{
    for (auto &pair : _image_inventory)
    {
        MTL::Texture *texture = reinterpret_cast<MTL::Texture *>(pair.second);

        if(texture)
        {
            texture->release();
        }
    }

    _image_inventory.clear();
}

bool metal_image_library_t::load_image(const char *name, const char *full_path)
{
    boden::asset::stb_ref_t stb(full_path);
    
    if(stb)
    {
        MTL::TextureDescriptor *desc = MTL::TextureDescriptor::alloc()->init();
        desc->setTextureType(MTL::TextureType2D);
        desc->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
        desc->setWidth(stb.width);
        desc->setHeight(stb.height);
        desc->setMipmapLevelCount(1);

        MTL::Texture *texture = _device->newTexture(desc);
        MTL::Region region = MTL::Region::Make2D(0, 0, stb.width, stb.height);
        texture->replaceRegion(region, 0, stb.data, stb.width * stb.number_of_channels);

        _image_inventory[name] = reinterpret_cast<boden::asset::texture_id_t>(texture);

        return true;
    }
    
    return false;
}

boden::asset::texture_id_t metal_image_library_t::get_image(const char *name)
{
    return reinterpret_cast<boden::asset::texture_id_t>(_image_inventory[name]);
}

} // platform
