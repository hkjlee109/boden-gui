#include "toolbox_view_controller.hpp"

namespace app {

toolbox_view_controller_t::toolbox_view_controller_t()
    : boden::widget::view_controller_t()
{
    init();
}

toolbox_view_controller_t::toolbox_view_controller_t(const boden::layout::rect_t &frame)
    : boden::widget::view_controller_t(frame)
{
    init();
}

toolbox_view_controller_t::~toolbox_view_controller_t()
{
}

void toolbox_view_controller_t::mouse_down(const boden::event_t &ev)
{
    view_controller_t::mouse_down(ev);
}

void toolbox_view_controller_t::mouse_dragged(const boden::event_t &ev)
{
}

void toolbox_view_controller_t::mouse_up(const boden::event_t &ev)
{
    view_controller_t::mouse_up(ev);
}

void toolbox_view_controller_t::draw(boden::builder_t &builder)
{
    for(const std::shared_ptr<boden::widget::view_t> &view : _view->get_subviews())
    {
        view->draw(builder);
    }
}

void toolbox_view_controller_t::on_button_click(void *sender)
{
    printf("# main_view_controller_t::on_button_click\n");
}

void toolbox_view_controller_t::init()
{
    _rectangle = std::make_shared<boden::widget::button_t>(boden::layout::rect_t(10, 10, 50, 50));
    _rectangle->set_image(std::make_unique<boden::widget::base::image_t>("rectangle"));
    _rectangle->add_target(this,
                        &toolbox_view_controller_t::on_button_click,
                        boden::widget::control_event_t::mouse_down);
    _view->add_subview(_rectangle);
    
    _image_view = std::make_shared<boden::widget::image_view_t>(boden::layout::rect_t(10, 70, 50, 50));
    _image_view->set_image(std::make_unique<boden::widget::base::image_t>("gearshape"));
    _image_view->set_tint_color({0xFF, 0xFF, 0x00, 0xFF});
    _view->add_subview(_image_view);
}

} // app
