#include "button.hpp"
#include <boden/renderer.hpp>

namespace boden {
namespace widget {

button_t::button_t()
    : control_t{},
      _bordered{true},
      _content_tint_color{0x00, 0x00, 0x00, 0xFF},
      _image_position{image_position_t::no_image}
{
}

button_t::button_t(const boden::layout::rect_t &frame)
    : control_t{frame},
      _bordered{true},
      _content_tint_color{0x00, 0x00, 0x00, 0xFF},
      _image_position{image_position_t::no_image}
{
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

    if(_image) 
    {
        boden::layout::point_t origin = convert_point_to_view({0, 0}, nullptr);
        boden::layout::rect_t frame{origin, _frame.size};

        builder.push_clip_rect({frame.origin.x, frame.origin.y, frame.size.width, frame.size.height});
    
        builder.add_image(_image->texture_id, 
                          {frame.origin.x, frame.origin.y}, 
                          {frame.origin.x + frame.size.width, frame.origin.y + frame.size.height},
                          _content_tint_color);
    }

    builder.pop_clip_rect();

    if(_bordered)
    {
        boden::layout::point_t origin = convert_point_to_view({0, 0}, nullptr);
        boden::layout::rect_t frame{origin, _frame.size};

        builder.push_clip_rect({frame.origin.x - get_layer_border_width() / 2, 
                                frame.origin.y - get_layer_border_width() / 2, 
                                frame.size.width + get_layer_border_width(), 
                                frame.size.height + get_layer_border_width()});
    
        builder.add_rect({frame.origin.x, frame.origin.y}, 
                         {frame.origin.x + frame.size.width, frame.origin.y + frame.size.height},
                         get_layer_border_color(),
                         get_layer_border_width());
    
        builder.pop_clip_rect();
    }
}

void button_t::mouse_down(const boden::event_t &ev)
{
    send_actions(boden::widget::control_event_t::mouse_down);
}

bool button_t::is_bordered() const
{
    return _bordered;
}

void button_t::set_bordered(bool bordered)
{
    _bordered = bordered;
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

const std::string & button_t::get_title() const
{
    return _title;
}

void button_t::set_title(std::string &title)
{
    _title = title;
}
    
} // widget
} // boden
