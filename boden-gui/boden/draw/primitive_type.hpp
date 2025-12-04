#pragma once

#include <cstdint>

namespace boden {
namespace draw {

enum class primitive_type_t : uint32_t
{
    triangle_strip,
    line
};

} // draw
} // boden
