#pragma once

#include <boden/context.hpp>
#include <boden/geometry/vec.hpp>
#include <boden/draw/command.hpp>
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
    
    void add_rect(boden::geometry::vec2_t &min, boden::geometry::vec2_t &max);

protected:
    std::vector<boden::draw::command_t> _commands;
    std::vector<boden::draw::vertex_t> _vertices;
    std::vector<boden::geometry::vec2_t> _path; 
    
private:
    void path_line_to(boden::geometry::vec2_t &to);
    void path_stroke();
};

} // boden
