#include "view_controller.hpp"

namespace boden {

view_controller_t::view_controller_t() 
    : _view{std::make_unique<boden::view_t>()}
{
}

view_controller_t::~view_controller_t()
{
}

} // boden