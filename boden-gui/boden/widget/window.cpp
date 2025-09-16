#include "window.hpp"

#include <boden/widget/view.hpp>
#include <boden/widget/view_controller.hpp>

namespace boden {
namespace widget {

window_t::window_t() 
    : _content_view{std::make_shared<boden::widget::view_t>()},
      _backend{nullptr}
{
}

window_t::window_t(const boden::layout::rect_t &frame)
    : _content_view{std::make_shared<boden::widget::view_t>(frame)},
      _backend{nullptr}
{
}

window_t::~window_t()
{
}

void window_t::mouse_down(const boden::event_t &ev)
{
    auto target = _content_view->hit_test(ev.location);
    if(target == nullptr)
    {
        return;
    }
    target->mouse_down(ev);
}

void window_t::mouse_dragged(const boden::event_t &ev)
{
    auto target = _content_view->hit_test(ev.location);
    if(target == nullptr)
    {
        return;
    }
    target->mouse_dragged(ev);
}

void window_t::mouse_moved(const boden::event_t &ev)
{

}

void window_t::mouse_up(const boden::event_t &ev)
{
    auto target = _content_view->hit_test(ev.location);
    if(target == nullptr)
    {
        return;
    }
    target->mouse_up(ev);
}

void window_t::set_backend(boden::backend_t *backend)
{
    _backend = backend;
}

void window_t::set_needs_display(bool needs)
{
    if(_backend)
    {
        _backend->set_needs_display(needs);
        return;
    }
}

void window_t::order_front() 
{
    _content_view->set_window(this->shared_from_this());
}

} // widget
} // boden
