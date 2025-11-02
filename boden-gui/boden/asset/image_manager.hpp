#pragma once

#include <boden/layout/size.hpp>
#include <boden/graphic/texture_manager.hpp>
#include <string>

namespace boden {
namespace asset {

class image_manager_t
{
public:
    image_manager_t();
    ~image_manager_t();

    boden::graphic::texture_id_t get_texture_id(const std::string &key) const;
    boden::layout::size_t get_texture_size(const std::string &key) const;
    void set_texture_manager(boden::graphic::texture_manager_t *manager);
    bool load(const std::string &key, const std::string &full_path);

private:
    boden::graphic::texture_manager_t *_texture_manager;
    std::unordered_map<std::string, boden::graphic::texture_id_t> _map;
};

} // asset
} // boden
