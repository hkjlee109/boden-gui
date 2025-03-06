#pragma once

#include <boden/layout/vec.hpp>
#include <cstdint>

namespace boden {
namespace draw {

struct vertex_t
{
    constexpr vertex_t(const boden::layout::vec2_t &_position, 
                       const boden::layout::vec2_t &_uv, 
                       const uint32_t &_color) 
        : position(_position), 
          uv(_uv),
          color(_color) {};

    boden::layout::vec2_t position;
    boden::layout::vec2_t uv;
    uint32_t color;
};

} // draw
} // boden
