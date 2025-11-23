#include "main_view_controller.hpp"

namespace miro {

main_view_controller_t::main_view_controller_t()
: boden::widget::view_controller_t()
{
}

main_view_controller_t::~main_view_controller_t()
{
}

void main_view_controller_t::load_view()
{
    boden::widget::view_controller_t::load_view();
    _view->set_frame(boden::layout::rect_t{0, 0, 640, 480});
    
    _canvas_ctrl = std::make_shared<miro::canvas_view_controller_t>();
    _view->add_subview(_canvas_ctrl->get_view());
    add_child_view_controller(_canvas_ctrl);
    
    _zoom_ctrl = std::make_shared<miro::zoom_view_controller_t>();
    _zoom_ctrl->set_zoom_delegate(this);
    _view->add_subview(_zoom_ctrl->get_view());
    add_child_view_controller(_zoom_ctrl);
}

void main_view_controller_t::did_zoom_change(uint32_t zoom)
{
    _canvas_ctrl->set_zoom(zoom);
}

} // miro
