#pragma once

#include <boden/math/vec.hpp>
#include <boden/draw/draw_command.hpp>
#include <boden/draw/draw_index.hpp>
#include <boden/draw/draw_vertex.hpp>
#include <vector>

namespace boden {

class renderer_t
{
public:
    renderer_t();
    virtual ~renderer_t();
    
    virtual void draw_rect();

private:
    std::vector<boden::draw::draw_command_t> commands;
    std::vector<boden::draw::draw_vertex_t> vertices;
    std::vector<boden::draw::draw_index_t> indices;
};

} // boden
