#pragma once

#include <boden/builder.hpp>
#include <boden/context.hpp>
#include <boden/layout/vec.hpp>
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

    boden::builder_t builder;

private:
};

} // boden
