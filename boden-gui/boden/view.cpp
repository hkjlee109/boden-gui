#include "view.hpp"

namespace boden {

view_t::view_t()
    : responder_t{},
      _bounds{0, 0, 0, 0},
      _frame{0, 0, 0, 0}
{
}

view_t::view_t(const boden::layout::rect_t &frame)
    : _bounds{0, 0, frame.size.width, frame.size.height},
      _frame{frame}
{
}

view_t::~view_t()
{
}

void view_t::draw(boden::context_t &ctx)
{
}

std::shared_ptr<boden::view_t> view_t::hit_test(boden::layout::point_t point)
{
    if(!_frame.contains(point)) 
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

void view_t::add_subview(const std::shared_ptr<boden::view_t> &view)
{
    _subviews.push_back(view);
}

float view_t::get_layer_border_width() const
{
    return _layer.border_width;
}

const boden::layout::color_t & view_t::get_layer_border_color() const 
{
    return _layer.border_color;
}

const std::vector<std::shared_ptr<boden::view_t>> & view_t::get_subviews() const
{
    return _subviews;
}

void view_t::set_layer_border_width(float width)
{
    _layer.border_width = width;
}

} // boden
