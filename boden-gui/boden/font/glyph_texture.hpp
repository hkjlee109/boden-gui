#pragma once

#include <boden/layout/rect.hpp>
#include <boden/layout/vec.hpp>
#include <boden/gpu/texture_id.hpp>

namespace boden {
namespace font {

struct glyph_texture_t
{
    glyph_texture_t() = default;
    constexpr glyph_texture_t(boden::gpu::texture_id_t _tid) 
        : tid{_tid},
          rect{},
          uv_min{},
          uv_max{} {};

    boden::gpu::texture_id_t tid;
    boden::layout::rect_t rect;
    boden::layout::vec2_t uv_min;
    boden::layout::vec2_t uv_max;
};

} // draw
} // boden