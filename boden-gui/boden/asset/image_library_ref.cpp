#include "image_library_ref.hpp"

namespace boden {
namespace asset {

std::unique_ptr<boden::asset::image_library_t> image_library_ref_t::_instance = nullptr;

image_library_ref_t::image_library_ref_t(boden::asset::image_library_t *instance)
{
    _instance.reset(instance);
}

boden::asset::image_library_t * image_library_ref_t::get_instance()
{
    return _instance.get();
}

} // asset
} // boden
