#pragma once

#include <boden/font/font_atlas.hpp>
#include <boden/font/font_metrics.hpp>
#include <boden/font/glyph_render_info.hpp>
#include <boden/gpu/texture_manager.hpp>
#include <cstdint>
#include <memory>

namespace boden {
namespace font {

class font_manager_t
{
public:
    font_manager_t();
    ~font_manager_t();

    const boden::font::font_metrics_t & get_font_metrics() const;
    void set_texture_manager(boden::gpu::texture_manager_t *manager);

    bool load(const std::string &full_path);
    
    void collect_glyph_render_info(const std::string &text, 
                                   std::vector<boden::font::glyph_render_info_t> &out);
                                   
private:
    boden::gpu::texture_manager_t *_texture_manager;
    std::vector<std::unique_ptr<boden::font::font_atlas_t>> _font_atlases;
};

} // font
} // boden
