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
}

void button_t::draw_rect(boden::builder_t &builder, const boden::layout::rect_t &dirty_rect)
{
    if(_hidden) 
    {
        return;
    }
    
    auto layer = get_layer();
    if(layer->get_texture_id() == 0)
    {
        return;
    }

    auto frame_in_window = convert_rect_to_view(_bounds, nullptr);

    builder.begin(layer->get_texture_id(), frame_in_window, dirty_rect);
    builder.add_rect_filled({_bounds.min_x(), _bounds.min_y()}, 
                            {_bounds.max_x(), _bounds.max_y()},
                            layer->get_background_color(), 
                            layer->get_corner_radius());

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
                width = std::round(_image->size.width * scale);
                height = std::round(_image->size.height * scale);
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
                          _content_tint_color);
    }

    if(layer->get_border_width() > 0)
    {
        builder.add_rect({_bounds.min_x(), _bounds.min_y()}, 
                         {_bounds.max_x(), _bounds.max_y()},
                         layer->get_border_color(),
                         layer->get_border_width());
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

void button_t::set_image_position(boden::widget::base::cell_image_position_t position)
{
    _image_position = position;
}

void button_t::set_image_scaling(boden::widget::base::image_scaling_t scaling)
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
    
void button_t::init(const boden::layout::rect_t &frame)
{
    boden::widget::control_t::init(frame);

    _content_tint_color = {0xFF, 0xFF, 0xFF, 0xFF};
    _image_position = boden::widget::base::cell_image_position_t::no_image;
    _image_scaling = boden::widget::base::image_scaling_t::none;

    auto layer = get_layer();
    layer->set_background_color({0x8F, 0x8F, 0x8F, 0xFF});
    layer->set_corner_radius(4);
}

} // widget
} // boden
