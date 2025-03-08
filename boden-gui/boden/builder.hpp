#pragma once

#include <boden/asset/texture_id.hpp>
#include <boden/layout/color.hpp>
#include <boden/layout/vec.hpp>
#include <boden/draw/command.hpp>
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

    void add_polyline(const std::vector<boden::layout::vec2_t> &path, 
                      const boden::layout::color_t &color, 
                      float thickness);
    
    void add_image(boden::asset::texture_id_t tid,
                   const boden::layout::vec2_t &p1, 
                   const boden::layout::vec2_t &p2);

    void reset();
    
    std::vector<boden::draw::command_t> commands;
    std::vector<boden::draw::vertex_t> vertices;

private:
};

} // boden
