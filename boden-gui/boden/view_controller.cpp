#include "view_controller.hpp"

namespace boden {

view_controller_t::view_controller_t() 
    : _view{std::make_shared<boden::view_t>()}
{
}

view_controller_t::view_controller_t(const boden::layout::rect_t &frame)
    : _view{std::make_shared<boden::view_t>(frame)}
{

}

view_controller_t::~view_controller_t()
{
}

boden::view_t & view_controller_t::get_view() const 
{
    if(_view == nullptr) 
    {
        throw std::runtime_error("Attempted to access a null view.");
    }

    return *_view;
}

} // boden