#include "www_backend.hpp"

namespace platform {
    
www_backend_t::www_backend_t()
{
    _window = std::make_shared<app::main_window_t>(boden::layout::rect_t{0, 0, 640, 480});
    _window->set_backend(this);
    _window->order_front();
}

www_backend_t::~www_backend_t()
{

}

void www_backend_t::draw(boden::builder_t &builder)
{
    _window->draw(builder);
}

void www_backend_t::try_run(const boden::event_t &event)
{
    switch(event.type)
    {
        case boden::event_type_t::draw:
            break;
                
        case boden::event_type_t::left_mouse_down:
            _window->mouse_down(event);
            break;
                
        case boden::event_type_t::left_mouse_dragged:
            _window->mouse_dragged(event);
            break;
                
        case boden::event_type_t::left_mouse_up:
            _window->mouse_up(event);
            break;
                
        case boden::event_type_t::mouse_moved:
            _window->mouse_moved(event);
            break;
    }
}

} // platform
