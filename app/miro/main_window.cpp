#include "main_window.hpp"

#include <miro/main_view_controller.hpp>

namespace miro {

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

} // miro
