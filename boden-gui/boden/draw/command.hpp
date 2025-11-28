#pragma once

#include <boden/graphic/compositing_operation.hpp>
#include <boden/draw/primitive_type.hpp>
#include <boden/graphic/texture_id.hpp>
#include <boden/layout/rect.hpp>
#include <cstdint>
#include <vector>

namespace boden {
namespace draw {

struct command_t
{
    command_t() = default;
    constexpr command_t(boden::draw::primitive_type_t _type,
                        uint32_t _count,
                        uint32_t _index_buffer_offset,
                        uint32_t _vertex_buffer_offset,
                        const boden::layout::rect_t &_clip_rect,
                        boden::graphic::texture_id_t _texture_id = 0)
        : type{_type},
          count{_count},
          index_buffer_offset{_index_buffer_offset},
          vertex_buffer_offset{_vertex_buffer_offset},
          clip_rect{_clip_rect},
          texture_id{_texture_id} {}

    boden::draw::primitive_type_t type;
    uint32_t count;
    uint32_t index_buffer_offset;
    uint32_t vertex_buffer_offset;

    boden::layout::rect_t clip_rect;
    boden::graphic::texture_id_t texture_id;
};

struct command_group_t
{
    command_group_t(boden::graphic::texture_id_t _tid, 
                              const boden::layout::rect_t &_frame,
                              boden::graphic::compositing_operation_t _operation)
        : tid{_tid},
          frame{_frame},
          operation{_operation} {}

    boden::graphic::texture_id_t tid;
    boden::layout::rect_t frame;
    boden::graphic::compositing_operation_t operation;

    std::vector<boden::draw::command_t> commands;
    uint8_t params[20];
};

} // draw
} // boden
