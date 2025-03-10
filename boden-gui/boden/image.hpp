#pragma once

#include <boden/asset/texture_id.hpp>

namespace boden {

struct image_t
{
public:
    image_t(const char *name);
    ~image_t();

    boden::asset::texture_id_t texture_id;
    
private:
};

} // boden
