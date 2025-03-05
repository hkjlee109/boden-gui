#pragma once

#include <boden/geometry/size.hpp>
#include <boden/geometry/vec.hpp>
#include <cstdint>

namespace boden {

class renderer_t;

using surface_handle_t = std::uint64_t;

struct context_t
{
    renderer_t *renderer;
    surface_handle_t surface_handle;
    
    boden::geometry::size_t display_size;
    boden::geometry::vec2_t display_scale;
};
    
} // boden
