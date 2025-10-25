#include "mtl_texture_manager.hpp"

#include <cassert>

namespace platform {

mtl_texture_manager_t::mtl_texture_manager_t(MTL::Device *device)
    : boden::gpu::texture_manager_t(),
      _device{device}
{
}

mtl_texture_manager_t::~mtl_texture_manager_t()
{
    for (auto& pair : _textures)
    {
        auto &texture = pair.second;
        if(!texture)
        {
            continue;
        }

        auto gpu_texture = reinterpret_cast<MTL::Texture *>(texture->gpu_texture_handle);
        if(!gpu_texture)
        {
            continue;
        }

        gpu_texture->release();
        texture->gpu_texture_handle = 0;
    }

    _textures.clear();
}

void mtl_texture_manager_t::bake(boden::gpu::gpu_texture_handle_t gpu_texture_handle,
                                 boden::layout::rect_t rect,
                                 const uint8_t *bytes,
                                 size_t length,
                                 uint8_t number_of_channels)
{
    auto texture = reinterpret_cast<MTL::Texture *>(gpu_texture_handle);
    auto region = MTL::Region::Make2D(rect.origin.x,
                                      rect.origin.y,
                                      rect.size.width,
                                      rect.size.height);
    
    texture->replaceRegion(region,
                           0,
                           bytes,
                           rect.size.width * number_of_channels);
}

boden::gpu::gpu_texture_handle_t mtl_texture_manager_t::create_gpu_texture(boden::layout::size_t size, 
                                                                           uint8_t number_of_channels)
{
    MTL::PixelFormat pixel_format;
    switch(number_of_channels)
    {
    case 1:
        pixel_format = MTL::PixelFormatA8Unorm;
        break;
    
    case 4:
        pixel_format = MTL::PixelFormatRGBA8Unorm;
        break;

    default:
        assert(false && "Error: Not supported pixel format.");
    }

    MTL::TextureDescriptor *desc = MTL::TextureDescriptor::alloc()->init();
    desc->setTextureType(MTL::TextureType2D);
    desc->setPixelFormat(pixel_format);
    desc->setWidth(size.width);
    desc->setHeight(size.height);
    desc->setMipmapLevelCount(1);
    MTL::Texture *texture = _device->newTexture(desc);
    desc->release();
    
    return reinterpret_cast<boden::gpu::gpu_texture_handle_t>(texture);
}

void mtl_texture_manager_t::destroy_gpu_texture(boden::gpu::gpu_texture_handle_t handle)
{
}

} // platform
