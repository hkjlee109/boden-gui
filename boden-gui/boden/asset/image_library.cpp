#include "image_library.hpp"

namespace boden {
namespace asset {

image_library_t::~image_library_t()
{
}

bool image_library_t::load_image_from_path(const std::string &name, const std::string &path)
{
    return false;
}

bool image_library_t::load_image_from_data(const std::string &name, const boden::image_t &image)
{
    return false;
}

} // asset
} // boden
