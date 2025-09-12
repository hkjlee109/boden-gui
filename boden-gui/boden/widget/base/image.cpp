#include "image.hpp"

#include <boden/asset/image_id_lookup_table.hpp>

namespace boden {
namespace widget {
namespace base {

image_t::image_t()
{
}

image_t::image_t(const char *name)
{
    if(auto *table = boden::asset::image_id_lookup_table_ref_t::get_instance())
    {
        texture_id = table->at(name);
    }
}

image_t::~image_t()
{
}

} // base
} // widget
} // boden
