#include "image.hpp"

#include <boden/asset/image_library_ref.hpp>

namespace boden {
namespace widget {
namespace base {

image_t::image_t()
{
}

image_t::image_t(const char *name)
{
    if(boden::asset::image_library_t *library = boden::asset::image_library_ref_t::get_instance())
    {
        texture_id = library->get_texture_id(name);
    }
}

image_t::~image_t()
{
}

} // base
} // widget
} // boden
