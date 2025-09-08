#include "main_view_controller.hpp"

#include <boden/layout/rect.hpp>
#include <boden/renderer.hpp>

namespace app {

main_view_controller_t::main_view_controller_t()
    : boden::widget::view_controller_t()
{
}

main_view_controller_t::main_view_controller_t(const boden::layout::rect_t &frame)
    : boden::widget::view_controller_t(frame)
{
}

main_view_controller_t::~main_view_controller_t()
{
}

void main_view_controller_t::mouse_down(const boden::event_t &ev)
{
    for(auto ctrl : _child_view_controllers)
    {
        ctrl->mouse_down(ev);
    }
}

void main_view_controller_t::mouse_dragged(const boden::event_t &ev)
{
    for(auto ctrl : _child_view_controllers)
    {
        ctrl->mouse_dragged(ev);
    }
}

void main_view_controller_t::mouse_up(const boden::event_t &ev)
{    
    for(auto ctrl : _child_view_controllers)
    {
        ctrl->mouse_up(ev);
    }
}

void main_view_controller_t::draw(boden::context_t &ctx)
{
    ctx.renderer->begin_draw(ctx);
    
    for(const std::shared_ptr<boden::widget::view_t> &view : _view->get_subviews())
    {
        view->draw(ctx.renderer->builder);
    }

    ctx.renderer->end_draw(ctx);
}

void main_view_controller_t::on_button_click(void *sender)
{
    printf("# main_view_controller_t::on_button_click\n");
}

void main_view_controller_t::load_view()
{
    _button = std::make_shared<boden::widget::button_t>(boden::layout::rect_t(10, 10, 50, 50));
    _button->set_layer_border_width(2);
    _button->set_layer_border_color({0x00, 0xFF, 0xFF, 0xFF});
    _button->add_target(this,
                        &main_view_controller_t::on_button_click,
                        boden::widget::control_event_t::mouse_down);
    _view->add_subview(_button);
    
    _image_view = std::make_shared<boden::widget::image_view_t>(boden::layout::rect_t(10, 70, 50, 50));
    _image_view->set_image(std::make_unique<boden::widget::base::image_t>("gearshape"));
    _image_view->set_tint_color({0xFF, 0xFF, 0x00, 0xFF});
    _view->add_subview(_image_view);
    
    auto canvas = std::make_shared<app::canvas_view_controller_t>(boden::layout::rect_t(70, 0, 570, 480));
    _view->add_subview(canvas->get_view());
    add_child_view_controller(canvas);
}

} // app
