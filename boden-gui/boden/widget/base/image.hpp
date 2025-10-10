#pragma once

#include <boden/gpu/texture_id.hpp>
#include <boden/layout/size.hpp>
#include <string>
#include <vector>

namespace boden {
namespace widget {
namespace base {

struct image_t
{
public:
    image_t();
    explicit image_t(const char *key);
    ~image_t();

    std::vector<uint8_t> data;
    boden::layout::size_t size;
    boden::gpu::texture_id_t texture_id;
    std::string key;

private:
};

} // base
} // widget
} // boden
