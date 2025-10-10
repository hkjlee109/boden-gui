#include "image_manager.hpp"

#include <boden/asset/stb_ref.hpp>

namespace boden {
namespace asset {

image_manager_t::image_manager_t()
{
}

image_manager_t::~image_manager_t()
{
}

std::optional<boden::gpu::texture_id_t> image_manager_t::get_texture_id(const std::string &key)
{
    auto it = _map.find(key);
    if(it == _map.end()) 
    {
        return std::nullopt;
    }
    return it->second;
}

void image_manager_t::set_texture_manager(boden::gpu::texture_manager_t *manager)
{
    _texture_manager = manager;
}

bool image_manager_t::load(const std::string &key, const std::string &full_path)
{
    if(!_texture_manager)
    {
        return false;
    }

    boden::asset::stb_ref_t stb{full_path.c_str()};
    if(!stb)
    {
        return false;
    }

    auto tid = _texture_manager->create({(float)stb.width, (float)stb.height}, 
                                       stb.number_of_channels);
    _texture_manager->load(tid, 
                           {0, 0, (float)stb.width, (float)stb.height}, 
                           stb.data, 
                           stb.width * stb.height * stb.number_of_channels);

    _map.insert({key, tid});
    return true;
}

} // asset
} // boden