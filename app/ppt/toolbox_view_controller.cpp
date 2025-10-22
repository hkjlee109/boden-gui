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
        ppt::shape_type_t shape_type;
        
        switch(view->get_tag())
        {
            case 0:
                shape_type = ppt::shape_type_t::rectangle;
                break;
                
            case 1:
                shape_type = ppt::shape_type_t::textbox;
                break;
            
            default:
                return;
        }
        
        _toolbox_delegate->did_toolbox_select(shared_from_this(), shape_type);
    }
}

void toolbox_view_controller_t::load_view()
{
    boden::widget::view_controller_t::load_view();
    _view->set_frame(boden::layout::rect_t(0, 0, 70, 480));
    
    _rectangle = std::make_shared<ppt::widget::hover_button_t>(boden::layout::rect_t(10, 10, 50, 40));

    _rectangle->layer.background_color = ppt::theme::color::background;
    _rectangle->set_image(std::make_unique<boden::widget::base::image_t>("rectangle"));
    _rectangle->set_content_tint_color({0xB7, 0xB7, 0xB7, 0xFF});
    _rectangle->set_hover_enabled(true);
    _rectangle->set_tag(0);
    _rectangle->add_target(this,
                           &toolbox_view_controller_t::on_tool_button_click,
                           boden::widget::control_event_t::mouse_down);
    _view->add_subview(_rectangle);

    _textbox = std::make_shared<ppt::widget::hover_button_t>(boden::layout::rect_t(10, 55, 50, 40));
    _textbox->layer.background_color = ppt::theme::color::background;
    _textbox->set_image(std::make_unique<boden::widget::base::image_t>("textbox"));
    _textbox->set_content_tint_color({0xB7, 0xB7, 0xB7, 0xFF});
    _textbox->set_hover_enabled(true);
    _textbox->set_tag(1);
    _textbox->add_target(this,
                         &toolbox_view_controller_t::on_tool_button_click,
                         boden::widget::control_event_t::mouse_down);
    _view->add_subview(_textbox);
    
    _image_view = std::make_shared<boden::widget::image_view_t>(boden::layout::rect_t(10, 420, 50, 50));
    _image_view->set_image(std::make_unique<boden::widget::base::image_t>("gearshape"));
    _image_view->set_tint_color({0xB7, 0xB7, 0xB7, 0xFF});
    _view->add_subview(_image_view);
}

void toolbox_view_controller_t::set_toolbox_delegate(ppt::toolbox_delegate_t *delegate)
{
    _toolbox_delegate = delegate;
}

} // ppt
