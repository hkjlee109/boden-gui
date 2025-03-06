#pragma once

#include <boden/geometry/vec.hpp>
#include <boden/draw/command.hpp>
#include <boden/draw/vertex.hpp>
#include <vector>

namespace boden {

class builder_t
{
public:
    builder_t();
    ~builder_t();
    
    void add_rect(const boden::geometry::vec2_t &p1, 
                  const boden::geometry::vec2_t &p2, 
                  float thickness);
    void add_polyline(const std::vector<boden::geometry::vec2_t> &path, float thickness);
    
    std::vector<boden::draw::command_t> commands;
    std::vector<boden::draw::vertex_t> vertices;

private:
};

} // boden
