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
                                                 parent_frame_in_window.size};
    
    if(_needs_display)
    {
        builder.begin(_tid, frame_in_window, _frame, boden::graphic::compositing_operation_t::clear);
        builder.end();

        builder.begin(_tid, frame_in_window, _frame, boden::graphic::compositing_operation_t::copy);

        float x = 0;
        float y = 0;
        float width = 0;
        float height = 0;

        boden::layout::rect_t bounds = _bounds.inset_by(_image_edge_insets);

        if(_image->size.width == 0 || _image->size.width == 0)
        {
            _image->size = builder.get_image_manager()->get_texture_size(_image->key);
        }

        switch(_image_scaling)
        {
            case boden::widget::base::image_scaling_t::none:
                x = bounds.mid_x() - _image->size.width / 2;
                y = bounds.mid_y() - _image->size.height / 2;
                width = _image->size.width;
                height = _image->size.height;
                break;

            case boden::widget::base::image_scaling_t::proportionally_down:
            {
                float scale = 1.0f;
                if(_image->size.width > bounds.size.width 
                   || _image->size.height > bounds.size.height) 
                {
                    float scale_x = bounds.size.width / _image->size.width;
                    float scale_y = bounds.size.height / _image->size.height;
                    scale = std::min(scale_x, scale_y);
                }
                width = std::round(_image->size.width * scale) + 2;
                height = std::round(_image->size.height * scale) + 2;
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
