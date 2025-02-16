#pragma once

#include <cstdint>

namespace boden {
namespace draw {

struct draw_command_t
{
    uint32_t vertex_offset;
    uint32_t index_offset;
};

} // draw
} // boden
