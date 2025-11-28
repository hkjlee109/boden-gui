#include "button.hpp"
#include <boden/renderer.hpp>

namespace boden {
namespace widget {

std::shared_ptr<button_t> button_t::alloc(const boden::layout::rect_t &frame)
{
    auto instance = std::make_shared<button_t>(frame);
    instance->init(frame);
    return instance;
}

button_t::button_t()
    : boden::widget::control_t{}
{
}

button_t::button_t(const boden::layout::rect_t &frame)
    : boden::widget::control_t{frame}
{
}

button_t::~button_t()
{
    auto tid = _image_layer->get_texture_id();
    if(tid)
    {
        if(auto window = _window.lock()) 
        {
            window->destroy_view_texture(tid);
        }
    }
}

void button_t::draw_rect(boden::builder_t &builder, const boden::layout::rect_t &dirty_rect)
{
    boden::widget::control_t::draw_rect(builder, dirty_rect);

    if(_hidden) 
    {
        return;
    }
    
    auto frame_in_window = convert_rect_to_view(_bounds, nullptr);
    _image_layer->draw(builder, frame_in_window);
}

void button_t::make_backing_layer()
{
    boden::widget::control_t::make_backing_layer();

    auto window = _window.lock();
    if(!window)
    {
        return;
    }

    auto tid = _image_layer->get_texture_id();
    if(tid)
    {
        window->destroy_view_texture(tid);
    }

    auto size = _image_layer->get_frame().size * _image_layer->get_contents_scale();
    _image_layer->set_texture_id(window->create_view_texture(size));
}

void button_t::view_will_move_to_window(std::shared_ptr<boden::widget::window_t> window)
{
    boden::widget::control_t::view_will_move_to_window(window);
    make_backing_layer();
}

void button_t::set_frame(const boden::layout::rect_t &frame)
{
    boden::widget::control_t::set_frame(frame);

    if(_image_layer->get_frame() == frame)
    {
        return;
    }

    _image_layer->set_frame({0, 0, frame.size.width, frame.size.height});    
    make_backing_layer();
}

void button_t::mouse_down(const boden::event_t &ev)
{
    send_actions(boden::widget::control_event_t::mouse_down);
}

void button_t::set_content_tint_color(const boden::layout::color_t &color)
{
    _content_tint_color = color;
    _image_layer->set_tint_color(color);
}

void button_t::set_image(std::unique_ptr<boden::widget::base::image_t> image)
{
    _image_layer->set_image(std::move(image));
}

void button_t::set_image_edge_insets(const boden::layout::edge_insets_t &insets)
{
    _image_layer->set_image_edge_insets(insets);
}

void button_t::set_image_position(boden::widget::base::cell_image_position_t position)
{
    _image_position = position;
}

void button_t::set_image_scaling(boden::widget::base::image_scaling_t scaling)
{
    _image_layer->set_image_scaling(scaling);
}

const std::string & button_t::get_title() const
{
    return _title;
}

void button_t::set_title(std::string &title)
{
    _title = title;
}
    
void button_t::init(const boden::layout::rect_t &frame)
{
    boden::widget::control_t::init(frame);

    _content_tint_color = {0xFF, 0xFF, 0xFF, 0xFF};
    _image_position = boden::widget::base::cell_image_position_t::no_image;

    _layer->set_background_color({0x8F, 0x8F, 0x8F, 0xFF});
    _layer->set_corner_radius(4);

    _image_layer = boden::widget::layer::image_layer_t::alloc({0, 0, frame.size.width, frame.size.height});
    _layer->add_layer(_image_layer);
}

} // widget
} // boden
