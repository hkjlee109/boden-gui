#pragma once

#include <boden/layout/size.hpp>
#include <boden/layout/vec.hpp>
#include <cstdint>

namespace boden {

class renderer_t;

using surface_handle_t = std::uint64_t;

struct context_t
{
    renderer_t *renderer;
    surface_handle_t surface_handle;
    
    boden::layout::size_t display_size;
    boden::layout::vec2_t display_scale;
};
    
} // boden
