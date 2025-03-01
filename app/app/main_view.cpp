#include "main_view.hpp"

namespace app {

main_view_t::main_view_t()
{
}

main_view_t::~main_view_t()
{
}

void main_view_t::draw(boden::renderer_t *renderer)
{
    renderer->begin_paint();
    
    renderer->end_paint();
}

} // platform
