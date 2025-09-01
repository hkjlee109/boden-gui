#include "mtl_image_library.hpp"
#include <boden/asset/stb_ref.hpp>

namespace platform {

mtl_image_library_t::mtl_image_library_t(MTL::Device *device)
    : boden::asset::image_library_t(),
      _device{device}
{
}

mtl_image_library_t::~mtl_image_library_t()
{
    for(auto &pair : _textures)
    {
        MTL::Texture *texture = reinterpret_cast<MTL::Texture *>(pair.second);

        if(texture)
        {
            texture->release();
        }
    }

    _textures.clear();
    _image_inventory.clear();
}

bool mtl_image_library_t::load_image_from_path(const std::string &name, const std::string &path)
{
    boden::asset::stb_ref_t stb(path.c_str());
    
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
        
        _textures[++_last_texture_id] = texture;
        _image_inventory[name] = _last_texture_id;

        return true;
    }
    
    return false;
}

bool mtl_image_library_t::load_image_from_data(const std::string &name, const boden::widget::base::image_t &image)
{
    MTL::TextureDescriptor *desc = MTL::TextureDescriptor::alloc()->init();
    desc->setTextureType(MTL::TextureType2D);
    desc->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    desc->setWidth(image.size.width);
    desc->setHeight(image.size.height);
    desc->setMipmapLevelCount(1);

    MTL::Texture *texture = _device->newTexture(desc);
    MTL::Region region = MTL::Region::Make2D(0, 0, image.size.width, image.size.height);
    texture->replaceRegion(region, 0, image.data.data(), image.size.width * 4);

    _textures[++_last_texture_id] = texture;
    _image_inventory[name] = _last_texture_id;
    
    return true;
}

boden::asset::texture_id_t mtl_image_library_t::get_texture_id(const char *name)
{
    return _image_inventory[name];
}

MTL::Texture * mtl_image_library_t::get_mtl_texture(boden::asset::texture_id_t texture_id)
{
    return _textures[texture_id];
}

} // platform
