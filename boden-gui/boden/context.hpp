#pragma once

#include <boden/batch.hpp>
#include <boden/layout/size.hpp>
#include <boden/layout/vec.hpp>
#include <cstdint>

namespace boden {

using surface_handle_t = std::uint64_t;

struct context_t
{
    surface_handle_t surface_handle;
    boden::layout::size_t display_size;
    float display_scale;
    
    std::shared_ptr<boden::batch_t> batch;
};
    
} // boden
