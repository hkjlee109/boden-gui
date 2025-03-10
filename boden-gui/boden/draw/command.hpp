#pragma once

#include <boden/asset/texture_id.hpp>
#include <boden/layout/rect.hpp>
#include <cstdint>

namespace boden {
namespace draw {

struct command_t
{
    constexpr command_t(uint32_t _vertex_count, 
                        uint32_t _vertex_offset,
                        const boden::layout::rect_t &_clip_rect,
                        boden::asset::texture_id_t _texture_id = 0)
        : vertex_count(_vertex_count), 
          vertex_offset(_vertex_offset),
          clip_rect{_clip_rect},
          texture_id{_texture_id} {}

    uint32_t vertex_count;
    uint32_t vertex_offset;
    
    boden::asset::texture_id_t texture_id;
    boden::layout::rect_t clip_rect;
};

} // draw
} // boden
