#include "view.hpp"

namespace boden {
namespace widget {

view_t::view_t()
    : responder_t{},
      _view_delegate{nullptr},
      _bounds{0, 0, 0, 0},
      _frame{0, 0, 0, 0},
      _hidden{false}
{
}

view_t::view_t(const boden::layout::rect_t &frame)
    : responder_t{},
      _view_delegate{nullptr},
      _bounds{0, 0, frame.size.width, frame.size.height},
      _frame{frame},
      _hidden{false}
{
}

view_t::~view_t()
{
}

void view_t::mouse_down(const boden::event_t &ev)
{
    if(_view_delegate) 
    {
        _view_delegate->did_view_mouse_down(shared_from_this(), ev.location);
    }
}
    
void view_t::mouse_dragged(const boden::event_t &ev)
{
    if(_view_delegate) 
    {
        _view_delegate->did_view_mouse_dragged(shared_from_this(), ev.location);
    }
}
    
void view_t::mouse_up(const boden::event_t &ev)
{
    if(_view_delegate) 
    {
        _view_delegate->did_view_mouse_up(shared_from_this(), ev.location);
    }
}

void view_t::set_view_delegate(boden::widget::view_delegate_t *delegate)
{
    _view_delegate = delegate;
}

const boden::layout::rect_t & view_t::get_frame() const
{
    return _frame;
}

void view_t::set_frame(const boden::layout::rect_t& frame)
{
    _frame = frame;
}

const std::vector<std::shared_ptr<boden::widget::view_t>> & view_t::get_subviews() const
{
    return _subviews;
}

std::shared_ptr<const boden::widget::view_t> view_t::get_superview() const 
{
    return _superview.lock();
}

void view_t::set_superview(const std::shared_ptr<const boden::widget::view_t> &view)
{
    _superview = view;
}

void view_t::set_window(const std::shared_ptr<boden::widget::window_t> &window)
{
    _window = window;
}

bool view_t::is_hidden() const
{
    return _hidden;
}

void view_t::set_hidden(bool hidden)
{
    _hidden = hidden;
}

void view_t::set_needs_display(bool needs)
{
    if(auto window = _window.lock()) 
    {
       window->set_needs_display(needs);
    }
}

void view_t::add_subview(const std::shared_ptr<boden::widget::view_t> &view)
{
    view->set_superview(this->shared_from_this());

    if(auto window = _window.lock()) 
    {
        view->set_window(window);
    }

    _subviews.push_back(view);
}

boden::layout::point_t view_t::convert_point_to_view(const boden::layout::point_t &point, 
                                                     const boden::widget::view_t *to_view) const 
{
    if(to_view == this) 
    {
        return {point.x + _frame.origin.x, point.y + _frame.origin.y};
    }

    if (auto superview = _superview.lock()) 
    {
        return superview->convert_point_to_view({point.x + _frame.origin.x, point.y + _frame.origin.y},
                                                to_view);
    }

    return {point.x + _frame.origin.x, point.y + _frame.origin.y};                                           
}

void view_t::draw(boden::builder_t &builder)
{
    for(auto it = _subviews.rbegin(); it != _subviews.rend(); ++it) 
    {
        auto subview = *it;
        if(subview) 
        {
            subview->draw(builder);
        }
    }
}

std::shared_ptr<boden::widget::view_t> view_t::hit_test(boden::layout::point_t point)
{
    if(_hidden) 
    {
        return nullptr;
    }

    boden::layout::point_t origin = convert_point_to_view({0, 0}, nullptr);
    boden::layout::rect_t frame{origin, _frame.size};

    if(!frame.contains(point)) 
    {
        return nullptr;
    }

    for(auto it = _subviews.rbegin(); it != _subviews.rend(); ++it) 
    {
        auto subview = *it;
        if(subview) 
        {
            auto hitView = subview->hit_test(point);
            if(hitView) 
            {
                return hitView;
            }
        }
    }

    return shared_from_this();
}

} // widget
} // boden
