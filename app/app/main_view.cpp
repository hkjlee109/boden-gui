#include "main_view.hpp"

#include <boden/geometry/rect.hpp>
#include <boden/renderer.hpp>

namespace app {

main_view_t::main_view_t()
    : boden::view_t()
{
    button_ = std::make_shared<boden::button_t>(boden::geometry::rect_t(100, 100, 100, 50));
    button_->layer.border_width = 4;

    subviews.push_back(button_);
}

main_view_t::~main_view_t()
{
}

void main_view_t::draw(boden::context_t &ctx)
{
    ctx.renderer->begin_draw(ctx);
    
    for(const std::shared_ptr<boden::view_t> &view : subviews)
    {
        view->draw(ctx);
    }

    ctx.renderer->end_draw(ctx);
}

} // platform
