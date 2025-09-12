#pragma once

#include <boden/asset/image_id_lookup_table.hpp>
#include <boden/asset/image_library.hpp>
#include <boden/asset/texture_id.hpp>
#include <boden/widget/base/image.hpp>
#include <Metal/Metal.hpp>
#include <string>
#include <vector>
#include <unordered_map>

namespace platform {

class mtl_image_library_t : public boden::asset::image_library_t
{
public:
    mtl_image_library_t(MTL::Device *device);
    ~mtl_image_library_t() override;
    
    bool load_image_from_path(const std::string &name, const std::string &path) override;
    bool load_image_from_data(const std::string &name, const boden::widget::base::image_t &image) override;

    MTL::Texture * get_mtl_texture(boden::asset::texture_id_t texture_id);
    void set_image_id_lookup_table(boden::asset::image_id_lookup_table_t *table);
    
private:
    MTL::Device *_device;
    
    uint32_t _last_texture_id;
    std::unordered_map<boden::asset::texture_id_t, MTL::Texture *> _textures;
    boden::asset::image_id_lookup_table_t *_image_id_lookup_table;
};

} // platform
