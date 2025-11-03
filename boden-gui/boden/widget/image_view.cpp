#include "image_view.hpp"

#include <boden/widget/layer/image_layer.hpp>

namespace boden {
namespace widget {

std::shared_ptr<image_view_t> image_view_t::alloc(const boden::layout::rect_t &frame)
{
    auto instance = std::make_shared<image_view_t>(frame);
    instance->init(frame);
    return instance;
}

image_view_t::image_view_t()
    : view_t()
{
}

image_view_t::image_view_t(const boden::layout::rect_t &frame)
    : view_t{frame}
{
}

image_view_t::~image_view_t()
{
}

void image_view_t::draw_rect(boden::builder_t &builder, const boden::layout::rect_t &dirty_rect)
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
    builder.end();

    auto image_layer = _layer->get_sublayers()[0];
    image_layer->draw(builder, frame_in_window);
}

void image_view_t::view_will_move_to_window(std::shared_ptr<boden::widget::window_t> window)
{
    boden::widget::view_t::view_will_move_to_window(window);
    
    create_image_layer_texture();
}

void image_view_t::set_frame(const boden::layout::rect_t &frame)
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

void image_view_t::set_image(std::unique_ptr<boden::widget::base::image_t> image)
{
    auto image_layer = std::dynamic_pointer_cast<boden::widget::layer::image_layer_t>(_layer->get_sublayers()[0]);
    if(!image_layer)
    {
        return;
    }

    image_layer->set_image(std::move(image));
}

const boden::layout::color_t & image_view_t::get_tint_color() const
{
    auto image_layer = std::dynamic_pointer_cast<boden::widget::layer::image_layer_t>(_layer->get_sublayers()[0]);
    if(!image_layer)
    {
        return boden::layout::color::none;
    }
    return image_layer->get_tint_color();
}

void image_view_t::set_tint_color(const boden::layout::color_t &color)
{
    auto image_layer = std::dynamic_pointer_cast<boden::widget::layer::image_layer_t>(_layer->get_sublayers()[0]);
    if(!image_layer)
    {
        return;
    }
    image_layer->set_tint_color(color);
}

void image_view_t::init(const boden::layout::rect_t &frame)
{
    boden::widget::view_t::init(frame);

    auto image_layer = boden::widget::layer::image_layer_t::alloc({0, 0, frame.size.width, frame.size.height});

    _layer->add_layer(image_layer);
}

void image_view_t::create_image_layer_texture()
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
