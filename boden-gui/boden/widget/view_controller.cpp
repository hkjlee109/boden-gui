#include "view_controller.hpp"

namespace boden {
namespace widget {

view_controller_t::view_controller_t() 
    : _view{std::make_shared<boden::widget::view_t>()}
{
    _parent.reset();
}

view_controller_t::view_controller_t(const boden::layout::rect_t &frame)
    : _view{std::make_shared<boden::widget::view_t>(frame)}
{
    _parent.reset();
}

view_controller_t::~view_controller_t()
{
}

std::shared_ptr<boden::widget::view_t> view_controller_t::get_view() const 
{
    if(_view == nullptr) 
    {
        throw std::runtime_error("Attempted to access a null view.");
    }

    return _view;
}

void view_controller_t::set_parent(const std::shared_ptr<const boden::widget::view_controller_t> &ctrl)
{
    _parent = ctrl;
}

void view_controller_t::add_child_view_controller(const std::shared_ptr<boden::widget::view_controller_t> &crtl)
{
    crtl->set_parent(this->shared_from_this());
    _child_view_controllers.push_back(crtl);
}

} // widget
} // boden
