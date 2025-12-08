#include "view.hpp"

#include <boden/widget/layout/layout_x_axis_anchor.hpp>
#include <cassert>

namespace boden {
namespace widget {

std::shared_ptr<view_t> view_t::alloc()
{
    auto instance = std::make_shared<view_t>();
    instance->init();
    return instance;
}

std::shared_ptr<view_t> view_t::alloc(const boden::layout::rect_t &frame)
{    
    auto instance = std::make_shared<view_t>(frame);
    instance->init(frame);
    return instance;
}

view_t::view_t()
    : responder_t{},
      _bounds{0, 0, 0, 0},
      _frame{0, 0, 0, 0},
      _hidden{false},
      _needs_display{false}
{
}

view_t::view_t(const boden::layout::rect_t &frame)
    : responder_t{},
      _bounds{0, 0, frame.size.width, frame.size.height},
      _frame{frame},
      _hidden{false},
      _needs_display{false}
{
}

view_t::~view_t()
{
    auto tid = _layer->get_texture_id();
    if(tid)
    {
        if(auto window = _window.lock()) 
        {
            window->destroy_view_texture(tid);
        }
    }
}

void view_t::draw_rect(boden::builder_t &builder, const boden::layout::rect_t &dirty_rect)
{
    if(_hidden) 
    {
        return;
    }

    auto frame_in_window = convert_rect_to_view(_bounds, nullptr);
    _layer->draw(builder, frame_in_window);

    for(auto subview : _subviews)
    {
        if(auto intersected = dirty_rect.intersection(subview->get_frame()))
        {
            const auto &rect = *intersected;
            auto &subview_frame = subview->get_frame();
            auto dirty_in_subview = boden::layout::rect_t{rect.origin.x - subview_frame.origin.x,
                                                          rect.origin.y - subview_frame.origin.y,
                                                          rect.size.width,
                                                          rect.size.height};

            subview->draw_rect(builder, dirty_in_subview);
        }
    }
}

boden::widget::view_ref_t view_t::hit_test(boden::layout::point_t point)
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

void view_t::make_backing_layer() 
{
    auto window = _window.lock();
    if(!window)
    {
        return;
    }

    auto tid = _layer->get_texture_id();
    if(tid)
    {
        window->destroy_view_texture(tid);
    }

    auto size = _layer->get_frame().size * _layer->get_contents_scale();
    _layer->set_texture_id(window->create_view_texture(size));
}

void view_t::view_did_change_backing_properties()
{
    auto window = _window.lock();
    if(!window)
    {
        return;
    }

    float new_scale = window->get_backing_scale_factor();
    if(new_scale != _layer->get_contents_scale())
    {
        _layer->set_contents_scale(window->get_backing_scale_factor());
        make_backing_layer();
    }

    for(auto &view : _subviews)
    {
        view->view_did_change_backing_properties();
    }
}

void view_t::view_will_move_to_window(std::shared_ptr<boden::widget::window_t> window)
{
    assert(_layer && "Error: _layer is null.");

    _window = window;
    _layer->set_contents_scale(window->get_backing_scale_factor());

    make_backing_layer();

    for(auto subview : _subviews)
    {
        subview->view_will_move_to_window(window);
    }
}

const boden::layout::rect_t & view_t::get_frame() const
{
    return _frame;
}

void view_t::set_frame(const boden::layout::rect_t &frame)
{
    if(_frame == frame)
    {
        return;
    }

    bool needs_backing_layer_recreate = (_frame.size != frame.size);

    _frame = frame;
    _bounds = {0, 0, frame.size.width, frame.size.height};

    _layer->set_frame(_bounds);

    if(needs_backing_layer_recreate)
    {
        make_backing_layer();
    }

    _needs_layout = true;
}

const boden::layout::rect_t & view_t::get_bounds() const
{
    return _bounds;
}

std::shared_ptr<boden::widget::layer::layer_t> view_t::get_layer()
{
    return _layer;
}

const std::vector<boden::widget::view_ref_t> & view_t::get_subviews() const
{
    return _subviews;
}

std::shared_ptr<const boden::widget::view_t> view_t::get_superview() const 
{
    return _superview.lock();
}

void view_t::set_superview(boden::widget::view_ref_t view)
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
    _needs_display = needs;
    _dirty_rect = _frame;

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

boden::widget::layout::layout_x_axis_anchor_ref_t view_t::get_leading_anchor() const
{
    return _leading_anchor;
}

const std::vector<std::shared_ptr<boden::widget::base::tracking_area_t>> & view_t::get_tracking_areas() const 
{
    return _tracking_areas;
}

void view_t::add_subview(boden::widget::view_ref_t view)
{
    view->set_superview(this->shared_from_this());

    if(auto window = _window.lock()) 
    {
        view->view_will_move_to_window(window);
    }

    _subviews.push_back(view);

    did_add_subview(view.get());
}

void view_t::remove_subview(boden::widget::view_ref_t view)
{
    _subviews.erase(std::remove_if(_subviews.begin(), 
                                   _subviews.end(),
                                   [view](const boden::widget::view_ref_t child) 
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

    if(auto superview = _superview.lock()) 
    {
        return superview->convert_point_to_view({point.x + _frame.origin.x, point.y + _frame.origin.y},
                                                to_view);
    }

    return {point.x + _frame.origin.x, point.y + _frame.origin.y};                                           
}

boden::layout::rect_t view_t::convert_rect_to_view(const boden::layout::rect_t &rect,
                                                   const boden::widget::view_t *to_view) const
{
    return 
    {
        convert_point_to_view(rect.origin, to_view),
        rect.size
    };
}

void view_t::layout()
{
    auto superview = _superview.lock();
    if(!superview)
    {
        return;
    }

    auto superview_frame = superview->get_frame();

}

void view_t::layout_if_needed()
{
    if(!_needs_layout)
    {
        return;
    }

    _needs_layout = false;
    layout_subtree_if_needed();
}

void view_t::layout_subtree_if_needed()
{
    layout();

    if(!_needs_layout)
    {
        return;
    }
    
    _needs_layout = false;

    for(auto subview : _subviews)
    {
        subview->layout_subtree_if_needed();
    }
}

void view_t::layout_subviews()
{
    for(auto subview : _subviews)
    {
        subview->layout_subviews();
    }
}

void view_t::update_constraints()
{

}

void view_t::enqueue_system_event(const boden::system_event_t &event)
{
    if(auto window = _window.lock()) 
    {
        window->enqueue_system_event(event);
    }   
}

void view_t::init()
{
    _layer = boden::widget::layer::layer_t::alloc();
}

void view_t::init(const boden::layout::rect_t &frame)
{
    _layer = boden::widget::layer::layer_t::alloc({0, 0, frame.size.width, frame.size.height});
    _leading_anchor = boden::widget::layout::layout_x_axis_anchor_t::alloc();
}

} // widget
} // boden
