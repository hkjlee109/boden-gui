#include "canvas_view_controller.hpp"
#include "canvas_view.hpp"

#include <boden/layout/rect.hpp>
#include <boden/renderer.hpp>
#include <boden/widget/shape/rectangle.hpp>

namespace app {

canvas_view_controller_t::canvas_view_controller_t()
    : boden::widget::view_controller_t()
{
    auto view = std::make_shared<app::canvas_view_t>();
    view->set_on_mouse_up(std::bind(&canvas_view_controller_t::on_background_mouse_up,
                                    this,
                                    std::placeholders::_1));
    _view = view;
    init();
}

canvas_view_controller_t::canvas_view_controller_t(const boden::layout::rect_t &frame)
    : boden::widget::view_controller_t(frame)
{
    auto view = std::make_shared<app::canvas_view_t>(frame);
    view->set_on_mouse_up(std::bind(&canvas_view_controller_t::on_background_mouse_up,
                                    this,
                                    std::placeholders::_1));
    _view = view;
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

void canvas_view_controller_t::on_background_mouse_up(void *sender)
{
    for(auto *shape : _selection)
    {
        if(shape)
        {
            shape->set_selected(false);
        }
    }

    _selection.clear();
}

void canvas_view_controller_t::on_shape_mouse_down(void *sender)
{
    auto *shape = static_cast<boden::widget::shape::shape_t *>(sender);
    
    if(!shape->is_selected())
    {
        shape->set_selected(true);
        _selection.insert(shape);
    }
}

void canvas_view_controller_t::init()
{
    auto rectangle{std::make_shared<boden::widget::shape::rectangle_t>(boden::layout::rect_t(150, 50, 100, 50))};
    rectangle->set_layer_background_color({0x00, 0x00, 0xFF, 0xFF});
    rectangle->set_layer_border_color({0x00, 0xFF, 0xFF, 0xFF});
    rectangle->set_layer_border_width(1);
    rectangle->add_target(this,
                          &canvas_view_controller_t::on_shape_mouse_down,
                          boden::widget::control_event_t::mouse_down);
    _shapes.push_back(rectangle);
    _view->add_subview(rectangle);
}

} // app
