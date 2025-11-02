#include "view_controller.hpp"

namespace boden {
namespace widget {

view_controller_t::view_controller_t() 
    : _backend{nullptr}
{
}

view_controller_t::~view_controller_t()
{
}

void view_controller_t::mouse_down(const boden::event_t &ev)
{
}

void view_controller_t::mouse_dragged(const boden::event_t &ev)
{
}

void view_controller_t::mouse_up(const boden::event_t &ev)
{
}

void view_controller_t::load_view()
{
    _view = boden::widget::view_t::alloc();
}

void view_controller_t::view_did_load()
{

}

std::shared_ptr<boden::widget::view_t> view_controller_t::get_view() 
{
    printf("# 1\n");
    if(_view == nullptr) 
    {
        printf("# 2\n");
        load_view();
        view_did_load();
        printf("# 3\n");
    }
    printf("# 4\n");
    return _view;
}

void view_controller_t::set_parent(std::shared_ptr<boden::widget::view_controller_t> ctrl)
{
    _parent = ctrl;
}

void view_controller_t::add_child_view_controller(std::shared_ptr<boden::widget::view_controller_t> crtl)
{
    crtl->set_parent(this->shared_from_this());
    _child_view_controllers.push_back(crtl);
}

} // widget
} // boden
