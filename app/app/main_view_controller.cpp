#include "main_view_controller.hpp"

#include <app/canvas_view_controller.hpp>
#include <app/toolbox_view_controller.hpp>

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

void main_view_controller_t::load_view()
{
    auto toolbox = std::make_shared<app::toolbox_view_controller_t>(boden::layout::rect_t(0, 0, 70, 480));
    _view->add_subview(toolbox->get_view());
    add_child_view_controller(toolbox);
    
    auto canvas = std::make_shared<app::canvas_view_controller_t>(boden::layout::rect_t(70, 0, 570, 480));
    _view->add_subview(canvas->get_view());
    add_child_view_controller(canvas);
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

void main_view_controller_t::draw(boden::builder_t &builder)
{
    for(const std::shared_ptr<boden::widget::view_t> &view : _view->get_subviews())
    {
        view->draw(builder);
    }
}

} // app
