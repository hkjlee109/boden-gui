#include "main_view_controller.hpp"

#include <boden/layout/rect.hpp>
#include <boden/renderer.hpp>

namespace app {

main_view_controller_t::main_view_controller_t()
    : boden::view_controller_t()
{
    _button = std::make_shared<boden::button_t>(boden::layout::rect_t(100, 100, 100, 50));
    _button->set_layer_border_width(4);
    _view->add_subview(_button);
    
    _image_view = std::make_shared<boden::image_view_t>(boden::layout::rect_t(300, 300, 60, 60));
    _image_view->set_image(std::make_unique<boden::image_t>("settings"));
    _image_view->set_tint_color({0xFF, 0x00, 0x00, 0xFF});
    _view->add_subview(_image_view);
}

main_view_controller_t::~main_view_controller_t()
{
}

void main_view_controller_t::draw(boden::context_t &ctx)
{
    ctx.renderer->begin_draw(ctx);
    
    for(const std::shared_ptr<boden::view_t> &view : _view->get_subviews())
    {
        view->draw(ctx);
    }

    ctx.renderer->end_draw(ctx);
}

void main_view_controller_t::mouse_down(const boden::event_t &ev)
{
    
}

void main_view_controller_t::mouse_up(const boden::event_t &ev)
{
    
}

} // platform
