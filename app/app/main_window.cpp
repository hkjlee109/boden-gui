#include "main_window.hpp"

#include <app/main_view_controller.hpp>

namespace app {

main_window_t::main_window_t()
    : boden::widget::window_t()
{
}

main_window_t::main_window_t(const boden::layout::rect_t &frame)
    : boden::widget::window_t(frame)
{
}

main_window_t::~main_window_t()
{
}

void main_window_t::order_front()
{
    boden::widget::window_t::order_front();
    set_content_view_controller(std::make_shared<app::main_view_controller_t>(_content_view->get_frame()));
}

void main_window_t::draw(boden::builder_t &builder)
{
    _content_view->draw(builder);
}

} // app
