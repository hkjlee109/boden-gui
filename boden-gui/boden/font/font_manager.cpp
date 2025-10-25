#include "font_manager.hpp"

#include <fstream>
#include <iostream>
namespace boden {
namespace font {

font_manager_t::font_manager_t()
{
}

font_manager_t::~font_manager_t()
{
}

const boden::font::font_metrics_t & font_manager_t::get_font_metrics() const
{
    return _font_atlases[0]->get_font_metrics();
}

void font_manager_t::set_texture_manager(boden::graphic::texture_manager_t *manager)
{
    _texture_manager = manager;
    
    for(auto &atlas : _font_atlases) 
    {
        atlas->set_texture_manager(_texture_manager);
    }
}

bool font_manager_t::load(const std::string &full_path)
{
    std::ifstream file{full_path, std::ios::binary | std::ios::ate};
    if(!file) 
    {
        return false;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> font(size);
    if(!file.read(reinterpret_cast<char*>(font.data()), size))
    {
        return false;
    }

    auto atlas = std::make_unique<font_atlas_t>();
    atlas->set_texture_manager(_texture_manager);
    atlas->load_font(font.data(), size);

    _font_atlases.push_back(std::move(atlas));
    return true;
}

void font_manager_t::collect_glyph_render_info(const std::string &text, 
                                               std::vector<boden::font::glyph_render_info_t> &out)
{
    _font_atlases[0]->collect_glyph_render_info(text, out);
}         

} // font
} // boden
