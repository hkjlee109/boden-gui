#pragma once

#include <boden/font/glyph_texture.hpp>
#include <boden/graphic/texture_id.hpp>

namespace boden {
namespace font {

struct glyph_render_info_t
{
    glyph_render_info_t() = default;
    constexpr glyph_render_info_t(const boden::font::glyph_texture_t &_texture,
                                  float _x_offset,
                                  float _y_offset,
                                  float _x_advance,
                                  float _y_advance) 
        : texture{_texture},
          x_offset{_x_offset},
          y_offset{_y_offset},
          x_advance{_x_advance},
          y_advance{_y_advance}
    {};

    boden::font::glyph_texture_t texture;
    float x_offset;
    float y_offset;
    float x_advance;
    float y_advance;
};

} // draw
} // boden
