#pragma once

#include <cstdint>

namespace boden {
namespace widget {
namespace base {

enum class layout_attribute_t : int32_t 
{
    none = 0,
    left,
    right,
    top,
    bottom,
    leading,
    trailing,
    width,
    height,
    center_x,
    center_y
};

} // base
} // widget
} // boden
