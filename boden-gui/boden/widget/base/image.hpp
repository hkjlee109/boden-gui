#pragma once

#include <boden/asset/texture_id.hpp>
#include <boden/layout/size.hpp>
#include <vector>

namespace boden {
namespace widget {
namespace base {

struct image_t
{
public:
    image_t();
    explicit image_t(const char *name);
    ~image_t();

    std::vector<uint8_t> data;
    boden::layout::size_t size;
    boden::asset::texture_id_t texture_id;

private:
};

} // base
} // widget
} // boden
