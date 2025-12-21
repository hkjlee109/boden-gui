#include "zoom_view_controller.hpp"

#include <boden/layout/edge_insets.hpp>
#include <miro/theme/color.hpp>

namespace miro {

#define MIN_ZOOM_INDEX      0
#define MAX_ZOOM_INDEX      13
#define DEFAULT_ZOOM_INDEX  10

static uint32_t steps[] = {
    1, 2, 3,
    5, 10, 15, 20,
    33,
    50, 75, 100, 125, 150,
    200
};

zoom_view_controller_t::zoom_view_controller_t()
    : boden::widget::view_controller_t(),
      _zoom_index{DEFAULT_ZOOM_INDEX}
{
}

zoom_view_controller_t::~zoom_view_controller_t()
{
}

void zoom_view_controller_t::load_view()
{
    boden::widget::view_controller_t::load_view();
    _view->set_frame(boden::layout::rect_t(10, 10, 100, 48));
    
    auto layer = _view->get_layer();
    layer->set_background_color(miro::theme::color::control_background);
    layer->set_corner_radius(4);
}

void zoom_view_controller_t::view_did_load()
{
    _zoom_in = miro::widget::action_button_t::alloc({8, 4, 40, 40});
    _zoom_in->get_layer()->set_background_color(miro::theme::color::control_background);
    _zoom_in->set_image(std::make_unique<boden::widget::base::image_t>("minus"));
    _zoom_in->set_image_edge_insets(boden::layout::edge_insets_t(8, 8, 8, 8));
    _zoom_in->set_image_scaling(boden::widget::base::image_scaling_t::proportionally_down);
    _zoom_in->set_content_tint_color(miro::theme::color::control_accent);
    _zoom_in->set_tracking_enabled(true);
    _zoom_in->add_target(this,
                         &zoom_view_controller_t::on_zoom_in_click,
                         boden::widget::control_event_t::mouse_down);
    _view->add_subview(_zoom_in);

    _zoom_out = miro::widget::action_button_t::alloc({52, 4, 40, 40});
    _zoom_out->get_layer()->set_background_color(miro::theme::color::control_background);
    _zoom_out->set_image(std::make_unique<boden::widget::base::image_t>("plus"));
    _zoom_out->set_image_edge_insets(boden::layout::edge_insets_t(8, 8, 8, 8));
    _zoom_out->set_image_scaling(boden::widget::base::image_scaling_t::proportionally_down);
    _zoom_out->set_content_tint_color(miro::theme::color::control_accent);
    _zoom_out->set_tracking_enabled(true);
    _zoom_out->add_target(this,
                         &zoom_view_controller_t::on_zoom_out_click,
                         boden::widget::control_event_t::mouse_down);
    _view->add_subview(_zoom_out);
}

void zoom_view_controller_t::on_zoom_in_click(void *sender)
{
    if(_zoom_delegate)
    {
        _zoom_index = (_zoom_index == MIN_ZOOM_INDEX) ? MIN_ZOOM_INDEX : (_zoom_index - 1);
        _zoom_delegate->did_zoom_change(shared_from_this(), steps[_zoom_index]);
    }
}

void zoom_view_controller_t::on_zoom_out_click(void *sender)
{
    if(_zoom_delegate)
    {
        _zoom_index = (_zoom_index == MAX_ZOOM_INDEX) ? MAX_ZOOM_INDEX : (_zoom_index + 1);
        _zoom_delegate->did_zoom_change(shared_from_this(), steps[_zoom_index]);
    }
}

void zoom_view_controller_t::set_zoom_delegate(miro::zoom_delegate_t *delegate)
{
    _zoom_delegate = delegate;
}

} // miro
