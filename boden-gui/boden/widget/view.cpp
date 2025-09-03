#include "view.hpp"

namespace boden {
namespace widget {

view_t::view_t()
    : responder_t{},
      _bounds{0, 0, 0, 0},
      _frame{0, 0, 0, 0},
      _hidden{false}
{
    _superview.reset();
}

view_t::view_t(const boden::layout::rect_t &frame)
    : responder_t{},
      _bounds{0, 0, frame.size.width, frame.size.height},
      _frame{frame},
      _hidden{false}
{
    _superview.reset();
}

view_t::~view_t()
{
}

void view_t::draw(boden::builder_t &builder)
{
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

void view_t::add_subview(const std::shared_ptr<boden::widget::view_t> &view)
{
    view->set_superview(this->shared_from_this());
    _subviews.push_back(view);
}

float view_t::get_layer_border_width() const
{
    return _layer.border_width;
}

void view_t::set_layer_border_width(float width)
{
    _layer.border_width = width;
}

const boden::layout::color_t & view_t::get_layer_background_color() const 
{
    return _layer.background_color;
}

void view_t::set_layer_background_color(const boden::layout::color_t &color)
{
    _layer.background_color = color;
}

const boden::layout::color_t & view_t::get_layer_border_color() const 
{
    return _layer.border_color;
}

void view_t::set_layer_border_color(const boden::layout::color_t &color)
{
    _layer.border_color = color;
}

const std::vector<std::shared_ptr<boden::widget::view_t>> & view_t::get_subviews() const
{
    return _subviews;
}

std::shared_ptr<const boden::widget::view_t> view_t::get_superview() const 
{
    return _superview.lock();
}

void view_t::set_superview(std::shared_ptr<const boden::widget::view_t> view)
{
    _superview = view;
}

bool view_t::is_hidden() const
{
    return _hidden;
}

void view_t::set_hidden(bool hidden)
{
    _hidden = hidden;
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

} // widget
} // boden
