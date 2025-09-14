#include "image.hpp"

#include <boden/asset/image_info_lookup_table.hpp>

namespace boden {
namespace widget {
namespace base {

image_t::image_t()
{
}

image_t::image_t(const char *name)
{
    if(auto *table = boden::asset::image_info_lookup_table_ref_t::get_instance())
    {
        auto info = table->at(name);
        texture_id = info.id;
        size.width = info.width;
        size.height = info.height;
    }
}

image_t::~image_t()
{
}

} // base
} // widget
} // boden
