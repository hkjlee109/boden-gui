#include "main_view.hpp"

#include <boden/layout/rect.hpp>
#include <boden/renderer.hpp>

namespace app {

main_view_t::main_view_t()
    : boden::view_t()
{
    _button = std::make_shared<boden::button_t>(boden::layout::rect_t(100, 100, 100, 50));
    _button->layer.border_width = 4;
    subviews.push_back(_button);
    
    _image_view = std::make_shared<boden::image_view_t>(boden::layout::rect_t(300, 300, 150, 150));
    _image_view->image.reset(new boden::image_t("settings"));
    subviews.push_back(_image_view);
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
