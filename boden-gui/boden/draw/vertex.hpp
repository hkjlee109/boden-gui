#pragma once

#include <boden/geometry/vec.hpp>
#include <cstdint>

namespace boden {
namespace draw {

struct vertex_t
{
    boden::geometry::vec2_t position;
    boden::geometry::vec2_t uv;
    uint32_t color;
};

} // draw
} // boden
