#pragma once

#include <boden/asset/texture_id.hpp>
#include <boden/batch.hpp>
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
    
    void add_rect(const boden::layout::vec2_t &p1, 
                  const boden::layout::vec2_t &p2,
                  const boden::layout::color_t &color, 
                  float thickness);

    void add_rect_filled(const boden::layout::vec2_t &p1, 
                         const boden::layout::vec2_t &p2,
                         const boden::layout::color_t &color);

    void add_polyline(const std::vector<boden::layout::vec2_t> &path, 
                      const boden::layout::color_t &color, 
                      float thickness);
    
    void add_image(boden::asset::texture_id_t tid,
                   const boden::layout::vec2_t &p1, 
                   const boden::layout::vec2_t &p2,
                   const boden::layout::color_t &color);

    void push_clip_rect(const boden::layout::rect_t &rect);
    void pop_clip_rect();

    const boden::batch_t & get_batch() const;
    const boden::layout::rect_t & get_clip_rect_top() const;

    void reset();

private:
    std::vector<boden::layout::rect_t> _clip_rect_stack;
    boden::batch_t _batch;
};

} // boden
