#include "toolbox_view_controller.hpp"

#include <miro/theme/color.hpp>

namespace miro {

toolbox_view_controller_t::toolbox_view_controller_t()
    : boden::widget::view_controller_t()
{
}

toolbox_view_controller_t::~toolbox_view_controller_t()
{
}

void toolbox_view_controller_t::on_pen_click(void *sender)
{
}

void toolbox_view_controller_t::on_selector_click(void *sender)
{
    if(_toolbox_delegate)
    {
        boden::widget::view_t *view = static_cast<boden::widget::view_t*>(sender);
        _toolbox_delegate->did_tool_select(shared_from_this(),
                                           static_cast<miro::tool_type_t>(view->get_tag()));
    }
}

void toolbox_view_controller_t::on_sticky_click(void *sender)
{
}

void toolbox_view_controller_t::on_text_click(void *sender)
{
}

void toolbox_view_controller_t::load_view()
{
    boden::widget::view_controller_t::load_view();
    _view->set_frame(boden::layout::rect_t(8, 100, 48, 300));
    
    auto layer = _view->get_layer();
    layer->set_background_color(miro::theme::color::control_background);
    layer->set_corner_radius(4);
}

void toolbox_view_controller_t::view_did_load()
{
    _selector = miro::widget::hover_button_t::alloc({4, 4, 40, 40});
    _selector->get_layer()->set_background_color(miro::theme::color::background);
    _selector->set_image(std::make_unique<boden::widget::base::image_t>("selector"));
    _selector->set_image_edge_insets(boden::layout::edge_insets_t(10, 10, 10, 10));
    _selector->set_image_scaling(boden::widget::base::image_scaling_t::proportionally_down);
    _selector->set_content_tint_color({0x00, 0x00, 0x00, 0xFF});
    _selector->set_hover_enabled(true);
    _selector->set_tag(static_cast<uint32_t>(miro::tool_type_t::selector));
    _selector->add_target(this,
                          &toolbox_view_controller_t::on_selector_click,
                          boden::widget::control_event_t::mouse_down);
    _view->add_subview(_selector);
    
    _sticky = miro::widget::hover_button_t::alloc({4, 48, 40, 40});
    _sticky->get_layer()->set_background_color(miro::theme::color::background);
    _sticky->set_image(std::make_unique<boden::widget::base::image_t>("sticky"));
    _sticky->set_image_edge_insets(boden::layout::edge_insets_t(10, 10, 10, 10));
    _sticky->set_image_scaling(boden::widget::base::image_scaling_t::proportionally_down);
    _sticky->set_content_tint_color({0x00, 0x00, 0x00, 0xFF});
    _sticky->set_hover_enabled(true);
    _sticky->set_tag(static_cast<uint32_t>(miro::tool_type_t::sticky));
    _sticky->add_target(this,
                        &toolbox_view_controller_t::on_sticky_click,
                        boden::widget::control_event_t::mouse_down);
    _view->add_subview(_sticky);
    
    _text = miro::widget::hover_button_t::alloc({4, 92, 40, 40});
    _text->get_layer()->set_background_color(miro::theme::color::background);
    _text->set_image(std::make_unique<boden::widget::base::image_t>("fonts"));
    _text->set_image_edge_insets(boden::layout::edge_insets_t(8, 8, 8, 8));
    _text->set_image_scaling(boden::widget::base::image_scaling_t::proportionally_down);
    _text->set_content_tint_color({0x00, 0x00, 0x00, 0xFF});
    _text->set_hover_enabled(true);
    _text->set_tag(static_cast<uint32_t>(miro::tool_type_t::text));
    _text->add_target(this,
                      &toolbox_view_controller_t::on_text_click,
                      boden::widget::control_event_t::mouse_down);
    _view->add_subview(_text);
    
    _pen = miro::widget::hover_button_t::alloc({4, 136, 40, 40});
    _pen->get_layer()->set_background_color(miro::theme::color::background);
    _pen->set_image(std::make_unique<boden::widget::base::image_t>("pencil"));
    _pen->set_image_edge_insets(boden::layout::edge_insets_t(10, 10, 10, 10));
    _pen->set_image_scaling(boden::widget::base::image_scaling_t::proportionally_down);
    _pen->set_content_tint_color({0x00, 0x00, 0x00, 0xFF});
    _pen->set_hover_enabled(true);
    _pen->set_tag(static_cast<uint32_t>(miro::tool_type_t::pen));
    _pen->add_target(this,
                     &toolbox_view_controller_t::on_pen_click,
                     boden::widget::control_event_t::mouse_down);
    _view->add_subview(_pen);
}

void toolbox_view_controller_t::set_toolbox_delegate(miro::toolbox_delegate_t *delegate)
{
    _toolbox_delegate = delegate;
}

} // miro
