#include "view.hpp"

namespace boden {
namespace widget {

view_t::view_t()
    : responder_t{},
      _bounds{0, 0, 0, 0},
      _frame{0, 0, 0, 0},
      _hidden{false}
{
}

view_t::view_t(const boden::layout::rect_t &frame)
    : responder_t{},
      _bounds{0, 0, frame.size.width, frame.size.height},
      _frame{frame},
      _hidden{false}
{
}

view_t::~view_t()
{
}

void view_t::draw(boden::builder_t &builder)
{
    for(auto subview : _subviews)
    {
        subview->draw(builder);
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
            auto view = subview->hit_test(point);
            if(view) 
            {
                return view;
            }
        }
    }

    return shared_from_this();
}

void view_t::did_add_subview(const boden::widget::view_t *view)
{
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

void view_t::set_superview(std::shared_ptr<boden::widget::view_t> view)
{
    _superview = view;
}

std::shared_ptr<const boden::widget::view_t> view_t::get_view_with_tag(uint32_t tag) const
{
    if(_hidden) 
    {
        return nullptr;
    }

    if(_tag == tag) 
    {
        return shared_from_this();
    }

    for(auto it = _subviews.rbegin(); it != _subviews.rend(); ++it) 
    {
        auto subview = *it;
        if(subview) 
        {
            auto view = subview->get_view_with_tag(tag);
            if(view) 
            {
                return view;
            }
        }
    }

    return nullptr;
}

std::shared_ptr<boden::widget::window_t> view_t::get_window() const
{
    return _window.lock();
}

void view_t::set_window(std::shared_ptr<boden::widget::window_t> window)
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

void view_t::set_needs_layout(bool needs)
{
    _needs_layout = needs;
}

uint32_t view_t::get_tag() const
{
    return _tag;
}

void view_t::set_tag(uint32_t tag)
{
    _tag = tag;
}

const std::vector<std::shared_ptr<boden::widget::base::tracking_area_t>> & view_t::get_tracking_areas() const 
{
    return _tracking_areas;
}

void view_t::add_subview(std::shared_ptr<boden::widget::view_t> view)
{
    view->set_superview(this->shared_from_this());

    if(auto window = _window.lock()) 
    {
        view->set_window(window);
    }

    _subviews.push_back(view);

    did_add_subview(view.get());
}

void view_t::remove_subview(std::shared_ptr<boden::widget::view_t> view)
{
    _subviews.erase(std::remove_if(_subviews.begin(), 
                                   _subviews.end(),
                                   [view](const std::shared_ptr<boden::widget::view_t> &child) 
                                   {
                                       return child.get() == view.get();
                                   }), 
                                   _subviews.end());
}

void view_t::remove_from_superview()
{
    if(auto super = _superview.lock()) 
    {
        super->remove_subview(shared_from_this());
        _superview.reset();
    }
}

void view_t::add_tracking_area(std::shared_ptr<boden::widget::base::tracking_area_t> area)
{
    if(std::find(_tracking_areas.begin(), _tracking_areas.end(), area) == _tracking_areas.end()) 
    {
        _tracking_areas.push_back(area);
    }
}

void view_t::remove_tracking_area(std::shared_ptr<boden::widget::base::tracking_area_t> area) {
    auto it = std::remove(_tracking_areas.begin(), _tracking_areas.end(), area);
    if(it != _tracking_areas.end()) 
    {
        _tracking_areas.erase(it, _tracking_areas.end());
    }
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

void view_t::layout_if_needed()
{
    if(_needs_layout)
    {
    }
}

void view_t::layout_subviews()
{
    for(auto subview : _subviews)
    {
        subview->layout_subviews();
    }
}

void view_t::enqueue_system_event(const boden::system_event_t &event)
{
    if(auto window = _window.lock()) 
    {
        window->enqueue_system_event(event);
    }   
}

} // widget
} // boden
