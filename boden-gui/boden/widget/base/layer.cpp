#include "layer.hpp"

namespace boden {
namespace widget {
namespace base {

layer_t::layer_t()
    : background_color{},
      border_color{},
      border_width{0},
      corner_radius{0},
      tid{0}
{
}

layer_t::~layer_t()
{
}

} // base
} // widget
} // boden
