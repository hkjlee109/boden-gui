#pragma once

#include <boden/context.hpp>

namespace boden {

class renderer_t
{
public:
    renderer_t();
    virtual ~renderer_t();
    
    virtual void render(boden::context_t &ctx);

private:
};

} // boden
