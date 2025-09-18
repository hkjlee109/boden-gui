#include "tracking_area.hpp"

namespace boden {
namespace widget {
namespace base {

tracking_area_t::tracking_area_t(boden::layout::rect_t rect,
                                 options options,
                                 std::shared_ptr<boden::widget::view_t> owner)
    : _rect{rect},
      _options{options},
      _owner{owner}
{
}

tracking_area_t::~tracking_area_t()
{
}

const boden::layout::rect_t & tracking_area_t::get_rect() const
{
    return _rect;
}

tracking_area_t::options tracking_area_t::get_options() const
{
    return _options;
}

std::weak_ptr<boden::widget::view_t> tracking_area_t::get_owner() const {
    return _owner;
}

} // base
} // widget
} // boden
