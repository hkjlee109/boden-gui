#include "main_view_controller.hpp"

#include <boden/widget/layout/dimension.hpp>
#include <boden/widget/layout/x_axis_anchor.hpp>
#include <boden/widget/layout/y_axis_anchor.hpp>
#include <miro/theme/color.hpp>

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
    
    auto layer = _view->get_layer();
    layer->set_background_color(miro::theme::color::grid_background);
    
    _canvas_ctrl = std::make_shared<miro::canvas_view_controller_t>();
    _view->add_subview(_canvas_ctrl->get_view());
    add_child_view_controller(_canvas_ctrl);
    
    _toolbox_ctrl = std::make_shared<miro::toolbox_view_controller_t>();
    _toolbox_ctrl->set_toolbox_delegate(this);
    _view->add_subview(_toolbox_ctrl->get_view());
    add_child_view_controller(_toolbox_ctrl);

    _zoom_ctrl = std::make_shared<miro::zoom_view_controller_t>();
    _zoom_ctrl->set_zoom_delegate(this);
    _view->add_subview(_zoom_ctrl->get_view());
    add_child_view_controller(_zoom_ctrl);
}

void main_view_controller_t::view_did_load()
{
    auto canvas_view = _canvas_ctrl->get_view();
    
    canvas_view->get_top_anchor()
               ->constraint_equal_to_anchor(_view->get_top_anchor())
               ->set_active(true);
    
    canvas_view->get_leading_anchor()
               ->constraint_equal_to_anchor(_view->get_leading_anchor())
               ->set_active(true);
    
    canvas_view->get_trailing_anchor()
               ->constraint_equal_to_anchor(_view->get_trailing_anchor())
               ->set_active(true);
    
    canvas_view->get_bottom_anchor()
               ->constraint_equal_to_anchor(_view->get_bottom_anchor())
               ->set_active(true);
    
    auto zoom_view = _zoom_ctrl->get_view();
    
    zoom_view->get_trailing_anchor()
               ->constraint_equal_to_anchor(_view->get_trailing_anchor(), -20)
               ->set_active(true);
    
    zoom_view->get_bottom_anchor()
               ->constraint_equal_to_anchor(_view->get_bottom_anchor(), -20)
               ->set_active(true);
    
    zoom_view->get_width_anchor()
               ->constraint_equal_to_constant(100)
               ->set_active(true);
    
    zoom_view->get_height_anchor()
               ->constraint_equal_to_constant(48)
               ->set_active(true);
}

void main_view_controller_t::did_tool_select(boden::widget::view_controller_ref_t sender,
                                             miro::tool_type_t type)
{
}

void main_view_controller_t::did_zoom_change(boden::widget::view_controller_ref_t sender,
                                             uint32_t zoom)
{
    _canvas_ctrl->set_zoom(zoom);
}

} // miro
