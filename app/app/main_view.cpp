#include "main_view.hpp"

#include <boden/renderer.hpp>

namespace app {

main_view_t::main_view_t()
    : boden::view_t()
{
}

main_view_t::~main_view_t()
{
}

void main_view_t::draw(boden::context_t &ctx)
{
    ctx.renderer->end_draw(ctx);
    
    ctx.renderer->end_draw(ctx);
}

} // platform
