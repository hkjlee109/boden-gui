#include "main_window.hpp"

#include <ppt/main_view_controller.hpp>

namespace ppt {

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
}

} // ppt
