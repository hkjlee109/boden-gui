#include "toolbox_view_controller.hpp"

#include <ppt/theme/color.hpp>

namespace ppt {

toolbox_view_controller_t::toolbox_view_controller_t()
    : boden::widget::view_controller_t()
{
}

toolbox_view_controller_t::~toolbox_view_controller_t()
{
}

void toolbox_view_controller_t::on_tool_button_click(void *sender)
{
    if(_toolbox_delegate)
    {
        boden::widget::view_t *view = static_cast<boden::widget::view_t*>(sender);
        _toolbox_delegate->did_tool_select(shared_from_this(),
                                              static_cast<ppt::shape_type_t>(view->get_tag()));
    }
}

void toolbox_view_controller_t::load_view()
{
    boden::widget::view_controller_t::load_view();
    _view->set_frame(boden::layout::rect_t(0, 0, 70, 480));
    
    _rectangle = ppt::widget::hover_button_t::alloc({10, 10, 50, 40});

    _rectangle->get_layer()->set_background_color(ppt::theme::color::background);
    _rectangle->set_image(std::make_unique<boden::widget::base::image_t>("rectangle"));
    _rectangle->set_image_scaling(boden::widget::base::image_scaling_t::proportionally_down);
    _rectangle->set_content_tint_color({0xB7, 0xB7, 0xB7, 0xFF});
    _rectangle->set_tracking_enabled(true);
    _rectangle->set_tag(static_cast<uint32_t>(ppt::shape_type_t::rectangle));
    _rectangle->add_target(this,
                           &toolbox_view_controller_t::on_tool_button_click,
                           boden::widget::control_event_t::mouse_down);
    _view->add_subview(_rectangle);

    _textbox = ppt::widget::hover_button_t::alloc({10, 55, 50, 40});
    _textbox->get_layer()->set_background_color(ppt::theme::color::background);
    _textbox->set_image(std::make_unique<boden::widget::base::image_t>("textbox"));
    _textbox->set_image_scaling(boden::widget::base::image_scaling_t::proportionally_down);
    _textbox->set_content_tint_color({0xB7, 0xB7, 0xB7, 0xFF});
    _textbox->set_tracking_enabled(true);
    _textbox->set_tag(static_cast<uint32_t>(ppt::shape_type_t::textbox));
    _textbox->add_target(this,
                         &toolbox_view_controller_t::on_tool_button_click,
                         boden::widget::control_event_t::mouse_down);
    _view->add_subview(_textbox);
    
    _image_view = boden::widget::image_view_t::alloc({10, 420, 50, 50});
    _image_view->set_image(std::make_unique<boden::widget::base::image_t>("gearshape"));
    _image_view->set_tint_color({0xB7, 0xB7, 0xB7, 0xFF});
    _view->add_subview(_image_view);
}

void toolbox_view_controller_t::set_toolbox_delegate(ppt::toolbox_delegate_t *delegate)
{
    _toolbox_delegate = delegate;
}

} // ppt
