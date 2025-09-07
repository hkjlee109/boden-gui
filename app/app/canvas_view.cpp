#include "canvas_view.hpp"

namespace app {

canvas_view_t::canvas_view_t()
: boden::widget::view_t()
{
}

canvas_view_t::canvas_view_t(const boden::layout::rect_t &frame)
: boden::widget::view_t(frame)
{
}

canvas_view_t::~canvas_view_t()
{
}

void canvas_view_t::mouse_down(const boden::event_t &ev)
{
    if(_on_mouse_down)
    {
        _on_mouse_down(this);
    }
}

void canvas_view_t::mouse_up(const boden::event_t &ev)
{
    if(_on_mouse_up)
    {
        _on_mouse_up(this);
    }
}

void canvas_view_t::set_on_mouse_down(std::function<void(void *)> callback)
{
    _on_mouse_down = callback;
}

void canvas_view_t::set_on_mouse_up(std::function<void(void *)> callback)
{
    _on_mouse_up = callback;
}
} // app
