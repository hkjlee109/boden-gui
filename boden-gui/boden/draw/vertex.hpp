#pragma once

#include <boden/math/vec.hpp>
#include <cstdint>

namespace boden {
namespace draw {

struct vertex_t
{
    boden::math::vec2_t position;
    boden::math::vec2_t uv;
    uint32_t color;
};

} // draw
} // boden
