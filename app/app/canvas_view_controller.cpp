#include "canvas_view_controller.hpp"

#include <boden/layout/rect.hpp>
#include <boden/renderer.hpp>

namespace app {

canvas_view_controller_t::canvas_view_controller_t()
    : boden::widget::view_controller_t()
{
    init();
}

canvas_view_controller_t::canvas_view_controller_t(const boden::layout::rect_t &frame)
    : boden::widget::view_controller_t(frame)
{
    init();
}

canvas_view_controller_t::~canvas_view_controller_t()
{
}

void canvas_view_controller_t::draw(boden::context_t &ctx)
{
    ctx.renderer->begin_draw(ctx);
    
    for(const std::shared_ptr<boden::widget::view_t> &view : _view->get_subviews())
    {
        view->draw(ctx.renderer->builder);
    }

    ctx.renderer->end_draw(ctx);
}

void canvas_view_controller_t::init()
{
    _rectangle = std::make_shared<boden::widget::shape::rectangle_t>(boden::layout::rect_t(300, 50, 100, 50));
    _rectangle->set_layer_background_color({0x00, 0x00, 0xFF, 0xFF});
    _rectangle->set_layer_border_color({0x00, 0xFF, 0xFF, 0xFF});
    _rectangle->set_layer_border_width(1);
    _view->add_subview(_rectangle);
}

} // platform
