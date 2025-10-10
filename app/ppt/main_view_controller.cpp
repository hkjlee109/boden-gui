#include "main_view_controller.hpp"

namespace ppt {

main_view_controller_t::main_view_controller_t()
    : boden::widget::view_controller_t()
{
}

main_view_controller_t::main_view_controller_t(const boden::layout::rect_t &frame)
    : boden::widget::view_controller_t(frame)
{
}

main_view_controller_t::~main_view_controller_t()
{
}

void main_view_controller_t::load_view()
{
    _toolbox_ctrl = std::make_shared<ppt::toolbox_view_controller_t>(boden::layout::rect_t(0, 0, 70, 480));
    _toolbox_ctrl->set_toolbox_delegate(this);
    _view->add_subview(_toolbox_ctrl->get_view());
    add_child_view_controller(_toolbox_ctrl);
    
    _canvas_ctrl = std::make_shared<ppt::canvas_view_controller_t>(boden::layout::rect_t(70, 0, 570, 480));
    _view->add_subview(_canvas_ctrl->get_view());
    add_child_view_controller(_canvas_ctrl);

    boden::widget::view_controller_t::load_view();
}

void main_view_controller_t::did_toolbox_select(std::shared_ptr<boden::widget::view_controller_t> sender,
                                                ppt::shape_type_t shape_type)
{
    _canvas_ctrl->create_shape(shape_type);
}

} // ppt
