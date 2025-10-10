#include "font_atlas.hpp"

#include <iostream>

namespace boden {
namespace font {

font_atlas_t::font_atlas_t()
    : _tid{0},
      _size{256, 256},
      _metrics{},
      _rect_packer{_size}
{
}

font_atlas_t::~font_atlas_t()
{
    if(_hb_font)
    {
        hb_font_destroy(_hb_font);
    }

    if(_ft_face)
    {
        FT_Done_Face(_ft_face);
    }

    if(_ft_library)
    {
        FT_Done_FreeType(_ft_library);
    } 
}

const boden::font::font_metrics_t & font_atlas_t::get_font_metrics() const
{
    return _metrics;
}

void font_atlas_t::set_texture_manager(boden::gpu::texture_manager_t *manager)
{
    _texture_manager = manager;
    _tid = _texture_manager->create(_size, 4);
}

void font_atlas_t::collect_glyph_render_info(const std::string &text, 
                                             std::vector<boden::font::glyph_render_info_t>& out)
{
    hb_buffer_t *hb_buffer = hb_buffer_create();

    hb_buffer_add_utf8(hb_buffer, text.c_str(), -1, 0, -1);

    hb_buffer_set_direction(hb_buffer, HB_DIRECTION_LTR);
    hb_buffer_set_script(hb_buffer, HB_SCRIPT_LATIN);
    hb_buffer_set_language(hb_buffer, hb_language_from_string("en", -1));

    hb_shape(_hb_font, hb_buffer, nullptr, 0);

    unsigned int glyph_count;
    hb_glyph_info_t *glyph_info = hb_buffer_get_glyph_infos(hb_buffer, &glyph_count);
    hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(hb_buffer, &glyph_count);

    for(size_t i = 0; i < glyph_count; ++i) 
    {
        hb_codepoint_t codepoint = glyph_info[i].codepoint;
        int x_offset = glyph_pos[i].x_offset / 64.0f;
        int y_offset = glyph_pos[i].y_offset / 64.0f;
        int x_advance = glyph_pos[i].x_advance / 64.0f;
        int y_advance = glyph_pos[i].y_advance / 64.0f;

        if(codepoint == 0) 
        {
            std::cerr << "Error: Font does not contain the glyph." << std::endl;
            continue;
        }

        boden::font::glyph_texture_t glyph_texture;
        
        if(!collect_glyph_texture(codepoint, glyph_texture))
        {
            continue;
        }

        out.emplace_back(glyph_texture, x_offset, y_offset, x_advance, y_advance);
    }

    hb_buffer_destroy(hb_buffer);
}

void font_atlas_t::load_font(const uint8_t *bytes, size_t size)
{
    _font_data.assign(bytes, bytes + size);

    if(FT_Init_FreeType(&_ft_library)) 
    {
        std::cerr << "Error: Failed to initialize FreeType." << std::endl;
        return;
    }

    if(FT_New_Memory_Face(_ft_library, _font_data.data(), _font_data.size(), 0, &_ft_face)) 
    {
        std::cerr << "Error: Failed to load font." << std::endl;
        return;
    }

    FT_Set_Char_Size(_ft_face, 0, 15 * 64, 0, 0);

    _hb_font = hb_ft_font_create(_ft_face, NULL);

    _metrics.ascender = _ft_face->size->metrics.ascender / 64.0f;
    _metrics.descender = _ft_face->size->metrics.descender / 64.0f;
}

bool font_atlas_t::collect_glyph_texture(uint32_t codepoint, boden::font::glyph_texture_t &out)
{
    auto it = _glyph_textures.find(codepoint);
    if(it == _glyph_textures.end()) 
    {
        if(FT_Load_Glyph(_ft_face, codepoint, FT_LOAD_DEFAULT)) 
        {
            std::cerr << "Error: Could not load glyph " << codepoint << std::endl;
            return false;
        }

        if(FT_Render_Glyph(_ft_face->glyph, FT_RENDER_MODE_NORMAL)) 
        {
            std::cerr << "Error: Could not render glyph " << codepoint << std::endl;
            return false;
        }

        FT_GlyphSlot slot = _ft_face->glyph;

        auto origin = _rect_packer.add_rect({(float)slot->bitmap.width, (float)slot->bitmap.rows});
        if(!origin.has_value())
        {
            std::cerr << "Error: Failed to find a free space." << std::endl;
            return false;
        }

        boden::layout::rect_t rect;
        rect.origin = *origin;
        rect.size = {(float)slot->bitmap.width, (float)slot->bitmap.rows};

        std::vector<uint8_t> rgba;
        rgba.resize(slot->bitmap.rows * slot->bitmap.pitch * 4);

        for(int y = 0; y < slot->bitmap.rows; ++y) 
        {
            for(int x = 0; x < slot->bitmap.width; ++x) 
            {
                uint8_t alpha = slot->bitmap.buffer[y * slot->bitmap.pitch + x];

                rgba[(y * slot->bitmap.width + x) * 4 + 0] = 255;
                rgba[(y * slot->bitmap.width + x) * 4 + 1] = 255;
                rgba[(y * slot->bitmap.width + x) * 4 + 2] = 255;
                rgba[(y * slot->bitmap.width + x) * 4 + 3] = alpha;
            }
        }

        if(!_texture_manager->load(_tid, rect, rgba.data(), rgba.size()))
        {
            std::cerr << "Error: Failed to load bitmap." << std::endl;
            return false;
        }

        out.tid = _tid;
        out.rect.origin = {(float)slot->bitmap_left, (float)slot->bitmap_top};
        out.rect.size = {(float)slot->bitmap.width, (float)slot->bitmap.rows};
        out.uv_min = {rect.origin.x / _size.width, rect.origin.y / _size.height};
        out.uv_max = {(rect.origin.x + rect.size.width) / _size.width, 
                      (rect.origin.y + rect.size.height) / _size.height};

        _glyph_textures.insert({codepoint, out});
        return true;
    }

    out = it->second;
    return true;
}

} // font
} // boden
