#pragma once

#include <boden/asset/image_library.hpp>

namespace boden {
namespace asset {

class image_library_ref_t
{
public:
    image_library_ref_t(boden::asset::image_library_t *instance);

    static boden::asset::image_library_t * get_instance();

private:
    static std::unique_ptr<boden::asset::image_library_t> _instance;
};

} // asset
} // boden
