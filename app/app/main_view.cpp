#include "main_view.hpp"

#include <boden/geometry/rect.hpp>
#include <boden/renderer.hpp>

namespace app {

main_view_t::main_view_t()
    : boden::view_t()
{
    _button = std::make_shared<boden::button_t>(boden::geometry::rect_t(0, 0, 100, 50));

    subviews.push_back(_button);
}

main_view_t::~main_view_t()
{
}

void main_view_t::draw(boden::context_t &ctx)
{
    ctx.renderer->begin_draw(ctx);
    
    for (const std::shared_ptr<boden::view_t> &view : subviews)
    {
        view->draw(ctx);
    }

    ctx.renderer->end_draw(ctx);
}

} // platform
