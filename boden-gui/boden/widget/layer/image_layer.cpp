#include "image_layer.hpp"

namespace boden {
namespace widget {
namespace layer {

std::shared_ptr<image_layer_t> image_layer_t::alloc()
{
    auto instance = std::make_shared<image_layer_t>();
    instance->init();
    return instance;
}

std::shared_ptr<image_layer_t> image_layer_t::alloc(const boden::layout::rect_t &frame)
{    
    auto instance = std::make_shared<image_layer_t>(frame);
    instance->init(frame);
    return instance;
}

image_layer_t::image_layer_t()
    : boden::widget::layer::layer_t{},
      _tint_color{}
{
}

image_layer_t::image_layer_t(const boden::layout::rect_t &frame)
    : boden::widget::layer::layer_t{frame},
      _tint_color{}
{
}

image_layer_t::~image_layer_t()
{
}

void image_layer_t::draw(boden::builder_t &builder, const boden::layout::rect_t &parent_frame_in_window)
{
    if(_tid == 0)
    {
        return;
    }

    auto frame_in_window = boden::layout::rect_t{parent_frame_in_window.origin + _frame.origin, 
                                                 parent_frame_in_window.size};
    
    if(_needs_display)
    {
        builder.begin(_tid, frame_in_window, _frame, boden::graphic::compositing_operation_t::clear);
        builder.add_image(_image->key, 
                         {_bounds.origin.x, _bounds.origin.y}, 
                         {_bounds.origin.x + _bounds.size.width, _bounds.origin.y + _bounds.size.height},
                         _tint_color);
        builder.end();
        _needs_display = false;
        return;
    }
    
    builder.begin(_tid, frame_in_window, _frame);         
    builder.end();
}

void image_layer_t::set_image(std::unique_ptr<boden::widget::base::image_t> image)
{
    _image = std::move(image);
    _needs_display = true;
}

const boden::layout::color_t & image_layer_t::get_tint_color() const
{
    return _tint_color;
}

void image_layer_t::set_tint_color(const boden::layout::color_t &color)
{
    _tint_color = color;
}

void image_layer_t::init()
{
}

void image_layer_t::init(const boden::layout::rect_t &frame)
{
    set_frame(frame);
}

} // layer
} // widget
} // boden
