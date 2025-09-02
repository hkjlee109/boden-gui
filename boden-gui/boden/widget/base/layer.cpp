#include "layer.hpp"

namespace boden {
namespace widget {
namespace base {

layer_t::layer_t()
    : background_color{0x00, 0x00, 0x00, 0x00},
      border_color{},
      border_width{0} 
{
}

layer_t::~layer_t()
{
}

} // base
} // widget
} // boden
