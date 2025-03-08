#pragma once

#include <boden/asset/texture_id.hpp>
#include <cstdint>

namespace boden {
namespace draw {

struct command_t
{
    constexpr command_t(uint32_t _vertex_count, 
                        uint32_t _vertex_offset,
                        boden::asset::texture_id_t _texture_id = 0)
        : vertex_count(_vertex_count), 
          vertex_offset(_vertex_offset),
          texture_id{_texture_id} {}

    uint32_t vertex_count;
    uint32_t vertex_offset;
    
    boden::asset::texture_id_t texture_id;
};

} // draw
} // boden
