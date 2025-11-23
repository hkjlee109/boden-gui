#pragma once

#include <boden/context.hpp>
#include <boden/graphic/texture_manager.hpp>

namespace boden {

class renderer_t
{
public:
    renderer_t();
    virtual ~renderer_t();
    
    virtual void render(boden::context_t &ctx);
    virtual void set_texture_manager(boden::graphic::texture_manager_t *manager);

protected:
    boden::graphic::texture_manager_t *_texture_manager;
};

} // boden
