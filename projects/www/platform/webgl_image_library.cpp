#include "webgl_image_library.hpp"

namespace platform {
    
extern "C" {

extern int lib_get_texture_id(const char *name);

} // "C"

webgl_image_library_t::webgl_image_library_t()
{

}

webgl_image_library_t::~webgl_image_library_t()
{

}

bool webgl_image_library_t::load_image_from_path(const std::string &name, const std::string &path)
{
    return true;
}

boden::asset::texture_id_t webgl_image_library_t::get_texture_id(const char *_name)
{
    return lib_get_texture_id(_name);
}

} // platform
