#include "zoom_view_controller.hpp"

#include <boden/layout/edge_insets.hpp>
#include <miro/theme/color.hpp>

namespace miro {

zoom_view_controller_t::zoom_view_controller_t()
    : boden::widget::view_controller_t()
{
}

zoom_view_controller_t::~zoom_view_controller_t()
{
}

void zoom_view_controller_t::load_view()
{
    boden::widget::view_controller_t::load_view();
    _view->set_frame(boden::layout::rect_t(10, 10, 100, 48));
    _view->layer.background_color = miro::theme::color::control_background;
    _view->layer.corner_radius = 4;
}

void zoom_view_controller_t::view_did_load()
{
    _zoom_in = std::make_shared<miro::widget::hover_button_t>(boden::layout::rect_t(8, 4, 40, 40));
    _zoom_in->layer.background_color = miro::theme::color::control_background;
    _zoom_in->set_image(std::make_unique<boden::widget::base::image_t>("minus"));
    _zoom_in->set_image_edge_insets(boden::layout::edge_insets_t(12, 12, 12, 12));
    _zoom_in->set_image_scaling(boden::widget::button_t::image_scaling_t::scale_proportionally_down);
    _zoom_in->set_content_tint_color(miro::theme::color::control_accent);
    _zoom_in->set_hover_enabled(true);
    _view->add_subview(_zoom_in);

    _zoom_out = std::make_shared<miro::widget::hover_button_t>(boden::layout::rect_t(52, 4, 40, 40));
    _zoom_out->layer.background_color = miro::theme::color::control_background;
    _zoom_out->set_image(std::make_unique<boden::widget::base::image_t>("plus"));
    _zoom_out->set_image_edge_insets(boden::layout::edge_insets_t(12, 12, 12, 12));
    _zoom_out->set_image_scaling(boden::widget::button_t::image_scaling_t::scale_proportionally_down);
    _zoom_out->set_content_tint_color(miro::theme::color::control_accent);
    _zoom_out->set_hover_enabled(true);
    _view->add_subview(_zoom_out);
}

} // miro
