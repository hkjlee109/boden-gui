#include "webgl_image_library.hpp"

namespace platform {
    
extern "C" {

// extern void lib_begin_draw();
// extern void lib_end_draw(boden::draw::command_t *commands,
//                          uint32_t commands_count,
//                          boden::draw::index_t *indices,
//                          uint32_t indices_count,
//                          boden::draw::vertex_t *vertices,
//                          uint32_t vertices_count);

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

boden::asset::texture_id_t webgl_image_library_t::get_texture_id(const char *name)
{
    return 0;
}

} // platform
