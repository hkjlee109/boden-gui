#pragma once

#include <boden/context.hpp>
#include <boden/gpu/texture_manager.hpp>

namespace boden {

class renderer_t
{
public:
    renderer_t();
    virtual ~renderer_t();
    
    virtual void render(boden::context_t &ctx);
    virtual void set_texture_manager(boden::gpu::texture_manager_t *manager);

protected:
    boden::gpu::texture_manager_t *_texture_manager;
};

} // boden
