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
      _image_scaling{boden::widget::base::image_scaling_t::none},
      _tint_color{0xFF, 0xFF, 0xFF, 0xFF}
{
}

image_layer_t::image_layer_t(const boden::layout::rect_t &frame)
    : boden::widget::layer::layer_t{frame},
      _image_scaling{boden::widget::base::image_scaling_t::none},
      _tint_color{0xFF, 0xFF, 0xFF, 0xFF}
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
                                                 parent_frame_in_window.size} * _contents_scale;
    auto frame = _frame * _contents_scale;
    
    if(!_needs_display)
    {
        builder.begin(_tid, frame_in_window, frame);         
        builder.end();
        return;
    }

    builder.begin(_tid, frame_in_window, frame, boden::graphic::compositing_operation_t::clear);
    builder.end();

    builder.begin(_tid, frame_in_window, frame, boden::graphic::compositing_operation_t::copy);

    float x = 0;
    float y = 0;
    float width = 0;
    float height = 0;

    auto bounds = _bounds.inset_by(_image_edge_insets) * _contents_scale;

    if(_image->size.width == 0 || _image->size.width == 0)
    {
        _image->size = builder.get_image_manager()->get_texture_size(_image->key);
    }

    auto image_width = _image->size.width * _contents_scale;
    auto image_height = _image->size.height * _contents_scale;

    switch(_image_scaling)
    {
        case boden::widget::base::image_scaling_t::none:
            x = bounds.mid_x() - image_width / 2;
            y = bounds.mid_y() - image_height / 2;
            width = image_width;
            height = image_height;
            break;

        case boden::widget::base::image_scaling_t::proportionally_down:
        {
            float scale = 1.0f;
            if(image_width > bounds.size.width 
               || image_height > bounds.size.height) 
            {
                float scale_x = bounds.size.width / image_width;
                float scale_y = bounds.size.height / image_height;
                scale = std::min(scale_x, scale_y);
            }

            width = std::round(image_width * scale);
            height = std::round(image_height * scale);
            x = std::round(bounds.mid_x() - width / 2);
            y = std::round(bounds.mid_y() - height / 2);
            break;
        }

        default:
            break;
    }

    builder.add_image(_image->key, 
                      {x, y}, 
                      {x + width, y + height},
                      _tint_color);
    builder.end();
    
    _needs_display = false;
}

void image_layer_t::set_image(std::unique_ptr<boden::widget::base::image_t> image)
{
    _image = std::move(image);
    _needs_display = true;
}

void image_layer_t::set_image_edge_insets(const boden::layout::edge_insets_t &insets)
{
    _image_edge_insets = insets;
    _needs_display = true;
}

void image_layer_t::set_image_scaling(boden::widget::base::image_scaling_t scaling)
{
    _image_scaling = scaling;
    _needs_display = true;
}

const boden::layout::color_t & image_layer_t::get_tint_color() const
{
    return _tint_color;
}

void image_layer_t::set_tint_color(const boden::layout::color_t &color)
{
    _tint_color = color;
    _needs_display = true;
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
