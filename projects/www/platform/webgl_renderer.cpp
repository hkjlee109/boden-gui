#include "webgl_renderer.hpp"

namespace platform {
    
extern "C" {

extern void lib_begin_draw();
extern void lib_end_draw();

} // "C"

webgl_renderer_t::webgl_renderer_t()
{

}

webgl_renderer_t::~webgl_renderer_t()
{

}

void webgl_renderer_t::begin_draw(boden::context_t &ctx)
{
    lib_begin_draw();
}

void webgl_renderer_t::end_draw(boden::context_t &ctx)
{
    lib_end_draw();
}

} // platform
