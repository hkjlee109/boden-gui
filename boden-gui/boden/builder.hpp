#pragma once

#include <boden/batch.hpp>
#include <boden/asset/image_manager.hpp>
#include <boden/font/font_manager.hpp>
#include <boden/graphic/texture_id.hpp>
#include <boden/layout/color.hpp>
#include <boden/layout/rect.hpp>
#include <boden/layout/vec.hpp>
#include <boden/draw/command.hpp>
#include <boden/draw/index.hpp>
#include <boden/draw/vertex.hpp>
#include <vector>

namespace boden {

class builder_t
{
public:
    builder_t();
    ~builder_t();
    
    std::shared_ptr<boden::batch_t> get_batch() const;
    const boden::layout::rect_t & get_clip_rect_top() const;

    void set_font_manager(boden::font::font_manager_t *_manager);

    const boden::asset::image_manager_t * get_image_manager() const;
    void set_image_manager(boden::asset::image_manager_t *_manager);

    void add_rect(const boden::layout::vec2_t &p1, 
                  const boden::layout::vec2_t &p2,
                  const boden::layout::color_t &color, 
                  float thickness);

    void add_rect_filled(const boden::layout::vec2_t &p1, 
                         const boden::layout::vec2_t &p2,
                         const boden::layout::color_t &color,
                         float corner_radius = 0);

    void add_polyline(const std::vector<boden::layout::vec2_t> &path, 
                      const boden::layout::color_t &color, 
                      float thickness);

    void add_image(const std::string &key,
                   const boden::layout::vec2_t &p1, 
                   const boden::layout::vec2_t &p2,
                   const boden::layout::color_t &color);

    void add_text(const std::string &text,
                 const boden::layout::vec2_t &p1, 
                 const boden::layout::vec2_t &p2,
                 const boden::layout::color_t &color);

    void begin(boden::graphic::texture_id_t tid, 
               const boden::layout::rect_t &frame,
               const boden::layout::rect_t &clip_rect);
    void end();

    void push_clip_rect(const boden::layout::rect_t &rect);
    void pop_clip_rect();

    void reset();

private:
    std::vector<boden::layout::rect_t> _clip_rect_stack;
    std::shared_ptr<boden::batch_t> _batch;

    boden::font::font_manager_t *_font_manager;
    boden::asset::image_manager_t *_image_manager;
};

} // boden
