#pragma once

#include <boden/gpu/texture_manager.hpp>
#include <optional>
#include <string>

namespace boden {
namespace asset {

class image_manager_t
{
public:
    image_manager_t();
    ~image_manager_t();

    std::optional<boden::gpu::texture_id_t> get_texture_id(const std::string &key);
    void set_texture_manager(boden::gpu::texture_manager_t *manager);
    bool load(const std::string &key, const std::string &full_path);

private:
    boden::gpu::texture_manager_t *_texture_manager;
    std::unordered_map<std::string, boden::gpu::texture_id_t> _map;
};

} // asset
} // boden
