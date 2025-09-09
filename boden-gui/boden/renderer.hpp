#pragma once

#include <boden/context.hpp>
#include <boden/layout/vec.hpp>
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
    
    virtual void render(const boden::context_t &ctx,
                        const std::vector<boden::draw::command_t> &commands,
                        const std::vector<boden::draw::index_t> &indices,
                        const std::vector<boden::draw::vertex_t> &vertices);

private:
};

} // boden
