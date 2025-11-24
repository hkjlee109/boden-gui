#include "main_view_controller.hpp"

#include <ppt/theme/color.hpp>

namespace ppt {

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
    _view->get_layer()->set_background_color(ppt::theme::color::background);

    _toolbox_ctrl = std::make_shared<ppt::toolbox_view_controller_t>();
    _toolbox_ctrl->set_toolbox_delegate(this);
    _view->add_subview(_toolbox_ctrl->get_view());
    add_child_view_controller(_toolbox_ctrl);

    _canvas_ctrl = std::make_shared<ppt::canvas_view_controller_t>();
    _view->add_subview(_canvas_ctrl->get_view());
    add_child_view_controller(_canvas_ctrl);
}

void main_view_controller_t::did_tool_select(boden::widget::view_controller_ref_t sender,
                                             ppt::shape_type_t shape_type)
{
    _canvas_ctrl->create_shape(shape_type);
}

} // ppt
