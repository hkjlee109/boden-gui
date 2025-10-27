#include "button.hpp"
#include <boden/renderer.hpp>

namespace boden {
namespace widget {

button_t::button_t()
    : control_t{}
{
    init();
}

button_t::button_t(const boden::layout::rect_t &frame)
    : control_t{frame}
{
    init();
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
    
    if(layer.tid == 0)
    {
        return;
    }

    boden::layout::rect_t frame_in_window = convert_rect_to_view(_bounds, nullptr);

    builder.begin(layer.tid, frame_in_window, dirty_rect);
    builder.add_rect_filled({_bounds.origin.x, _bounds.origin.y}, 
                            {_bounds.origin.x + _bounds.size.width, _bounds.origin.y + _bounds.size.height},
                            layer.background_color, 
                            layer.corner_radius);

    if(_image) 
    {
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
            case image_scaling_t::scale_none:
                x = bounds.mid_x() - _image->size.width / 2;
                y = bounds.mid_y() - _image->size.height / 2;
                width = _image->size.width;
                height = _image->size.height;
                break;

            case image_scaling_t::scale_proportionally_down:
            {
                float scale = 1.0f;
                if(_image->size.width > bounds.size.width 
                   || _image->size.height > bounds.size.height) 
                {
                    float scale_x = bounds.size.width / _image->size.width;
                    float scale_y = bounds.size.height / _image->size.height;
                    scale = std::min(scale_x, scale_y);
                }
                width = std::round(_image->size.width * scale);
                height = std::round(_image->size.height * scale);
                x = std::round(bounds.mid_x() - width / 2);
                y = std::round(bounds.mid_y() - height / 2);
                printf("# scale = %f\n", scale);
                break;
            }

            default:
                break;
        }

        builder.add_image(_image->key, 
                          {x, y}, 
                          {x + width, y + height},
                          _content_tint_color);
    }

    if(layer.border_width > 0)
    {
        builder.add_rect({_bounds.origin.x, _bounds.origin.y}, 
                         {_bounds.origin.x + _bounds.size.width, _bounds.origin.y + _bounds.size.height},
                         layer.border_color,
                         layer.border_width);
    }

    builder.end();
}

void button_t::mouse_down(const boden::event_t &ev)
{
    send_actions(boden::widget::control_event_t::mouse_down);
}

void button_t::set_content_tint_color(const boden::layout::color_t &color)
{
    _content_tint_color = color;
}

void button_t::set_image(std::shared_ptr<boden::widget::base::image_t> image)
{
    _image = image;
}

void button_t::set_image_edge_insets(const boden::layout::edge_insets_t &insets)
{
    _image_edge_insets = insets;
}

void button_t::set_image_position(image_position_t position)
{
    _image_position = position;
}

void button_t::set_image_scaling(image_scaling_t scaling)
{
    _image_scaling = scaling;
}

const std::string & button_t::get_title() const
{
    return _title;
}

void button_t::set_title(std::string &title)
{
    _title = title;
}
    
void button_t::init()
{
    _content_tint_color = {0xFF, 0xFF, 0xFF, 0xFF};
    _image_position = image_position_t::no_image;
    _image_scaling = image_scaling_t::scale_none;

    layer.background_color = {0x8F, 0x8F, 0x8F, 0xFF};
    layer.corner_radius = 4;
}

} // widget
} // boden
