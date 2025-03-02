#pragma once

#include <boden/context.hpp>
#include <boden/math/vec.hpp>
#include <boden/draw/command.hpp>
#include <boden/draw/index.hpp>
#include <boden/draw/vertex.hpp>
#include <vector>

namespace boden {

class renderer_t
{
public:
    renderer_t();
    virtual ~renderer_t();

    virtual void begin_draw(boden::context_t &ctx);
    virtual void end_draw(boden::context_t &ctx);
    
    virtual void draw_rect();

private:
    std::vector<boden::draw::command_t> commands;
    std::vector<boden::draw::vertex_t> vertices;
    std::vector<boden::draw::index_t> indices;
};

} // boden
