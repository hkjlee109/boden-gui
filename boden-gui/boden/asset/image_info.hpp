#pragma once

#include <boden/asset/texture_id.hpp>

namespace boden {
namespace asset {

struct image_info_t
{
    boden::asset::texture_id_t id;
    float width;
    float height;
};
    
} // asset
} // boden
