#include "texture_manager.hpp"

#include <cassert>
#include <iostream>

namespace boden {
namespace graphic {

texture_manager_t::texture_manager_t()
    : _last_texture_id{0}
{
}

texture_manager_t::~texture_manager_t()
{
    _textures.clear();
}

boden::graphic::gpu_texture_handle_t texture_manager_t::get_gpu_texture_handle(boden::graphic::texture_id_t tid)
{
    auto it = _textures.find(tid);
    if(it == _textures.end()) 
    {
        return 0;
    }
    texture_t &texture = *(it->second);

    if(!texture.gpu_texture_handle)
    {
        texture.gpu_texture_handle = create_gpu_texture(texture.size, texture.number_of_channels);
    }

    if(texture.needs_update)
    {
        std::vector<uint8_t> rect_data;
        rect_data.resize(texture.dirty_rect.size.width 
                         * texture.dirty_rect.size.height 
                         * texture.number_of_channels);

        size_t rect_stride = texture.dirty_rect.size.width * texture.number_of_channels;

        for(int row = 0; row < texture.dirty_rect.size.height; ++row) 
        {
            int src_y = texture.dirty_rect.origin.y + row;
            int src_x = texture.dirty_rect.origin.x;

            const uint8_t *src = texture.data.data() 
                                 + (src_y * (int)texture.size.width + src_x) * texture.number_of_channels;
            uint8_t *dst = rect_data.data() + row * rect_stride;
            std::memcpy(dst, src, rect_stride);
        }

        bake(texture.gpu_texture_handle,
             texture.dirty_rect,
             rect_data.data(),
             rect_data.size(),
             texture.number_of_channels);

        texture.needs_update = false;
    }

    return texture.gpu_texture_handle;
}

boden::layout::size_t texture_manager_t::get_texture_size(boden::graphic::texture_id_t tid) const
{
    auto it = _textures.find(tid);
    if(it == _textures.end()) 
    {
        return {0, 0};
    }

    return (it->second)->size;
}

texture_id_t texture_manager_t::create(boden::layout::size_t size,
                                       uint8_t number_of_channels)
{
    auto texture = std::make_unique<texture_t>(++_last_texture_id, size, number_of_channels);
    _textures.emplace(_last_texture_id, std::move(texture));
    return _last_texture_id;
}

void texture_manager_t::destroy(texture_id_t tid)
{
    auto it = _textures.find(tid);
    if (it != _textures.end())
    {
        _unused_textures.push_back(std::move(it->second));
        _textures.erase(it);
    }
}

bool texture_manager_t::load(boden::graphic::texture_id_t tid, 
                             boden::layout::rect_t rect,
                             const uint8_t *bytes, 
                             size_t length)
{
    auto it = _textures.find(tid);
    if(it == _textures.end()) 
    {
        return false;
    }
    texture_t &texture = *(it->second);

    texture.alloc_data_if_needed();

    assert(texture.data.size() >= (rect.origin.x + rect.size.width) 
                                  * (rect.origin.y + rect.size.height) 
                                  * texture.number_of_channels 
           && "Error: Texture is too small.");

    for(int row = 0; row < rect.size.height; ++row) 
    {
        uint8_t *dst = &texture.data[((rect.origin.y + row) * texture.size.width + rect.origin.x) * texture.number_of_channels];
        const uint8_t *src = bytes + (row * (int)rect.size.width * texture.number_of_channels);
        std::memcpy(dst, src, rect.size.width * texture.number_of_channels);
    }

    texture.dirty_rect = texture.needs_update ? texture.dirty_rect.unify_with(rect)
                                              : rect;

    texture.needs_update = true;
    return true;
}

void texture_manager_t::cleanup_unused_texture()
{
    if(_unused_textures.empty())
    {
        return;
    }

    for(auto &texure : _unused_textures)
    {
        if(texure && texure->gpu_texture_handle)
        {
            destroy_gpu_texture(texure->gpu_texture_handle);
        }
    }
    _unused_textures.clear();
}

} // graphic
} // boden
