#pragma once

#include <boden/layout/size.hpp>
#include <boden/layout/vec.hpp>
#include <cstdint>

namespace boden {

using surface_handle_t = std::uint64_t;

struct context_t
{
    surface_handle_t surface_handle;
    
    boden::layout::size_t display_size;
    boden::layout::vec2_t display_scale;
};
    
} // boden
