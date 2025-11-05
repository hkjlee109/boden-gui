#include "button.hpp"
#include <boden/renderer.hpp>
#include <boden/widget/layer/image_layer.hpp>

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
}

void button_t::draw_rect(boden::builder_t &builder, const boden::layout::rect_t &dirty_rect)
{
    if(_hidden) 
    {
        return;
    }
    
    auto layer = get_layer();
    auto tid = layer->get_texture_id();
    if(tid == 0)
    {
        return;
    }

    auto frame_in_window = convert_rect_to_view(_bounds, nullptr);

    builder.begin(tid, frame_in_window, dirty_rect);
    builder.add_rect_filled({_bounds.min_x(), _bounds.min_y()}, 
                            {_bounds.max_x(), _bounds.max_y()},
                            layer->get_background_color(), 
                            layer->get_corner_radius());

    if(layer->get_border_width() > 0)
    {
        builder.add_rect({_bounds.min_x(), _bounds.min_y()}, 
                         {_bounds.max_x(), _bounds.max_y()},
                         layer->get_border_color(),
                         layer->get_border_width());
    }
    builder.end();

    auto image_layer = _layer->get_sublayers()[0];
    image_layer->draw(builder, frame_in_window);
}

void button_t::view_will_move_to_window(std::shared_ptr<boden::widget::window_t> window)
{
    boden::widget::view_t::view_will_move_to_window(window);
    create_image_layer_texture();
}

void button_t::set_frame(const boden::layout::rect_t &frame)
{
    boden::widget::view_t::set_frame(frame);

    auto image_layer = _layer->get_sublayers()[0];
    if(!image_layer)
    {
        return;
    }

    if(image_layer->get_frame() == frame)
    {
        return;
    }

    image_layer->set_frame({0, 0, frame.size.width, frame.size.height});    
    create_image_layer_texture();
}

void button_t::mouse_down(const boden::event_t &ev)
{
    send_actions(boden::widget::control_event_t::mouse_down);
}

void button_t::set_content_tint_color(const boden::layout::color_t &color)
{
    _content_tint_color = color;

    auto image_layer = std::dynamic_pointer_cast<boden::widget::layer::image_layer_t>(_layer->get_sublayers()[0]);
    if(!image_layer)
    {
        return;
    }

    image_layer->set_tint_color(color);
}

void button_t::set_image(std::unique_ptr<boden::widget::base::image_t> image)
{
    auto image_layer = std::dynamic_pointer_cast<boden::widget::layer::image_layer_t>(_layer->get_sublayers()[0]);
    if(!image_layer)
    {
        return;
    }

    image_layer->set_image(std::move(image));
}

void button_t::set_image_edge_insets(const boden::layout::edge_insets_t &insets)
{
    auto image_layer = std::dynamic_pointer_cast<boden::widget::layer::image_layer_t>(_layer->get_sublayers()[0]);
    if(!image_layer)
    {
        return;
    }
    image_layer->set_image_edge_insets(insets);
}

void button_t::set_image_position(boden::widget::base::cell_image_position_t position)
{
    _image_position = position;
}

void button_t::set_image_scaling(boden::widget::base::image_scaling_t scaling)
{
    auto image_layer = std::dynamic_pointer_cast<boden::widget::layer::image_layer_t>(_layer->get_sublayers()[0]);
    if(!image_layer)
    {
        return;
    }
    image_layer->set_image_scaling(scaling);
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

    auto layer = get_layer();
    layer->set_background_color({0x8F, 0x8F, 0x8F, 0xFF});
    layer->set_corner_radius(4);

    auto image_layer = boden::widget::layer::image_layer_t::alloc({0, 0, frame.size.width, frame.size.height});
    _layer->add_layer(image_layer);
}

void button_t::create_image_layer_texture()
{
    if(auto window = _window.lock())
    {
        auto image_layer = std::dynamic_pointer_cast<boden::widget::layer::image_layer_t>(_layer->get_sublayers()[0]);
        if(!image_layer)
        {
            return;
        }

        auto tid = image_layer->get_texture_id();
        if(tid)
        {
            window->destroy_view_texture(tid);
        }

        image_layer->set_texture_id(window->create_view_texture(image_layer->get_frame().size));
    }
}

} // widget
} // boden
