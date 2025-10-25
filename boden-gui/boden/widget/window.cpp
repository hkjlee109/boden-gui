#include "window.hpp"

#include <boden/widget/view.hpp>
#include <boden/widget/view_controller.hpp>

namespace boden {
namespace widget {

window_t::window_t() 
    : _content_view{std::make_shared<boden::widget::view_t>()},
      _first_responder{nullptr},
      _backend{nullptr}
{
    _tracking_area_manager.set_content_view(_content_view);
}

window_t::window_t(const boden::layout::rect_t &frame)
    : _content_view{std::make_shared<boden::widget::view_t>(frame)},
      _first_responder{nullptr},
      _backend{nullptr}
{
    _tracking_area_manager.set_content_view(_content_view);
}

window_t::~window_t()
{
}

void window_t::order_front() 
{
}

void window_t::draw(boden::builder_t &builder)
{
    _content_view->draw_rect(builder, _content_view->get_frame());
}

void window_t::mouse_down(const boden::event_t &event)
{
    auto target = _content_view->hit_test(event.location);
    if(target == nullptr)
    {
        return;
    }
    target->mouse_down(event);
}

void window_t::mouse_dragged(const boden::event_t &event)
{
    auto target = _content_view->hit_test(event.location);
    if(target == nullptr)
    {
        return;
    }
    target->mouse_dragged(event);
}

void window_t::mouse_moved(const boden::event_t &event)
{
    _tracking_area_manager.mouse_moved(event);
}

void window_t::mouse_up(const boden::event_t &event)
{
    auto target = _content_view->hit_test(event.location);
    if(target == nullptr)
    {
        return;
    }
    target->mouse_up(event);
}

void window_t::key_down(const boden::event_t &event)
{
    if(_first_responder)
    {
        _first_responder->key_down(event);
    }
}

void window_t::key_up(const boden::event_t &event)
{
    if(_first_responder)
    {
        _first_responder->key_up(event);
    }
}

void window_t::scroll_wheel(const boden::event_t &event)
{
    if(_first_responder)
    {
        _first_responder->scroll_wheel(event);
    }
}

void window_t::set_backend(boden::backend_t *backend)
{
    _backend = backend;
}

void window_t::set_content_view(std::shared_ptr<boden::widget::view_t> view)
{
    _content_view = view;
    _content_view->view_will_move_to_window(this->shared_from_this());
    _tracking_area_manager.set_content_view(_content_view);
}

void window_t::set_content_view_controller(std::shared_ptr<boden::widget::view_controller_t> ctrl)
{
    _content_view_controller = ctrl;
    set_content_view(_content_view_controller->get_view());
    make_first_responder(nullptr);
}

void window_t::set_needs_display(bool needs)
{
    if(_backend)
    {
        _backend->set_needs_display(needs);
        return;
    }
}

void window_t::set_texture_manager(boden::gpu::texture_manager_t *texture_manager)
{
    _texture_manager = texture_manager;
}

void window_t::system(const boden::system_event_t &system_event)
{
    if(_first_responder)
    {
        _first_responder->system_event(system_event);
    }
}

void window_t::enqueue_system_event(const boden::system_event_t &system_event)
{
    _backend->enqueue_system_event(system_event);
}

bool window_t::make_first_responder(std::shared_ptr<boden::widget::base::responder_t> responder)
{
    if(responder)
    {
        _first_responder = responder;
        responder->become_first_responder();
    }
    else 
    {
        if(_first_responder)
        {
            _first_responder->resign_first_responder();
        }

        auto &views = _content_view->get_subviews();
        
        for(auto it = views.rbegin(); it != views.rend(); ++it) 
        {
            if(*it && (*it)->accepts_first_responder()) 
            {
                _first_responder = *it;
                return true;
            }
        }
        _first_responder = shared_from_this();
    }

    return true;
}

} // widget
} // boden
