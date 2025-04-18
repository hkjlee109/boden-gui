
#pragma once

#include <boden/asset/image_library.hpp>
#include <boden/asset/texture_id.hpp>
#include <boden/image.hpp>
#include <string>
#include <vector>

namespace platform {

class webgl_image_library_t : public boden::asset::image_library_t
{
public:
    webgl_image_library_t();
    ~webgl_image_library_t();
    
    bool load_image_from_path(const std::string &name, const std::string &path) override;
    boden::asset::texture_id_t get_texture_id(const char *name) override;

private:
};

} // platform