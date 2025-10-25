#pragma once

#include <boden/font/font_metrics.hpp>
#include <boden/font/glyph_render_info.hpp>
#include <boden/font/rect_packer.hpp>
#include <boden/graphic/texture_id.hpp>
#include <boden/graphic/texture_manager.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <hb.h>
#include <hb-ft.h>
#include <vector>
#include <unordered_map>

namespace boden {
namespace font {

class font_atlas_t
{
public:
    font_atlas_t();
    ~font_atlas_t();

    const boden::font::font_metrics_t & get_font_metrics() const;
    void set_texture_manager(boden::graphic::texture_manager_t *manager);

    void collect_glyph_render_info(const std::string &text, 
                                   std::vector<boden::font::glyph_render_info_t> &out);
                                   
    void load_font(const uint8_t *bytes, size_t size);

private:
    boden::graphic::texture_id_t _tid;
    boden::layout::size_t _size;
    boden::font::font_metrics_t _metrics;

    boden::font::rect_packer_t _rect_packer;
    boden::graphic::texture_manager_t *_texture_manager;
    std::unordered_map<uint32_t, boden::font::glyph_texture_t> _glyph_textures;

    std::vector<uint8_t> _font_data;
    FT_Library _ft_library;
    FT_Face _ft_face;
    hb_font_t *_hb_font;

    bool collect_glyph_texture(uint32_t codepoint, boden::font::glyph_texture_t &out);
};

} // font
} // boden
