#pragma once

#include <cstdint>

namespace boden {
namespace draw {

struct command_t
{
    constexpr command_t(const uint32_t &_vertex_count, 
                        const uint32_t &_vertex_offset)
        : vertex_count(_vertex_count), 
          vertex_offset(_vertex_offset) {}

    uint32_t vertex_count;
    uint32_t vertex_offset;
};

} // draw
} // boden
