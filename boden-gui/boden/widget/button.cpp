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

void button_t::draw(boden::builder_t &builder)
{
    if(_hidden) 
    {
        return;
    }

    boden::layout::point_t origin = convert_point_to_view({0, 0}, nullptr);
    boden::layout::rect_t frame{origin, _frame.size};
    
    builder.push_clip_rect({frame.origin.x - layer.border_width, 
                            frame.origin.y - layer.border_width, 
                            frame.size.width + layer.border_width * 2, 
                            frame.size.height + layer.border_width * 2});

    builder.add_rect_filled({frame.origin.x, frame.origin.y}, 
                            {frame.origin.x + frame.size.width, frame.origin.y + frame.size.height},
                            layer.background_color);

    if(_image) 
    {
        float x, y, width, height = 0;

        switch(_image_scaling)
        {
        case image_scaling_t::scale_none:
            x = frame.mid_x() - _image->size.width / 2;
            y = frame.mid_y() - _image->size.height / 2;
            width = _image->size.width;
            height = _image->size.height;
            break;
        default:
            break;
        }
    
        builder.add_image(_image->texture_id, 
                          {x, y}, 
                          {x + width, y + height},
                          _content_tint_color);
    }

    if(layer.border_width > 0)
    {
        boden::layout::point_t origin = convert_point_to_view({0, 0}, nullptr);
        boden::layout::rect_t frame{origin, _frame.size};
    
        builder.add_rect({frame.origin.x, frame.origin.y}, 
                         {frame.origin.x + frame.size.width, frame.origin.y + frame.size.height},
                         layer.border_color,
                         layer.border_width);
    }

    builder.pop_clip_rect();
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
}

} // widget
} // boden
