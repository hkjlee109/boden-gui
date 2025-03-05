#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "renderer.hpp"
#include <simd/simd.h>

namespace boden {

renderer_t::renderer_t()
{
}

renderer_t::~renderer_t()
{
}

void renderer_t::begin_draw(boden::context_t &ctx)
{
}

void renderer_t::end_draw(boden::context_t &ctx)
{
}

void renderer_t::add_rect(boden::geometry::vec2_t &min, boden::geometry::vec2_t &max)
{

}

void renderer_t::path_line_to(boden::geometry::vec2_t &to)
{

}

void renderer_t::path_stroke()
{

}

} // boden
