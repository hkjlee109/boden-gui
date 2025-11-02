#include "image_view.hpp"

namespace boden {
namespace widget {

std::shared_ptr<image_view_t> image_view_t::alloc(const boden::layout::rect_t &frame)
{
    auto instance = std::make_shared<image_view_t>(frame);
    instance->init(frame);
    return instance;
}

image_view_t::image_view_t()
    : view_t(),
      _tint_color{}
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
    
    boden::graphic::texture_id_t tid = get_layer()->get_texture_id();
    if(tid == 0)
    {
        return;
    }
    
    boden::layout::rect_t frame_in_window = convert_rect_to_view(_bounds, nullptr);

    builder.begin(tid, frame_in_window, dirty_rect, boden::graphic::compositing_operation_t::copy);

    builder.add_image(_image->key, 
                      {_bounds.origin.x, _bounds.origin.y}, 
                      {_bounds.origin.x + _bounds.size.width, _bounds.origin.y + _bounds.size.height},
                      _tint_color);
                      
    builder.end();
}

void image_view_t::set_image(std::unique_ptr<boden::widget::base::image_t> image)
{
    _image = std::move(image);
}

const boden::layout::color_t & image_view_t::get_tint_color() const
{
    return _tint_color;
}

void image_view_t::set_tint_color(const boden::layout::color_t &color)
{
    _tint_color = color;
}

void image_view_t::init(const boden::layout::rect_t &frame)
{
    boden::widget::view_t::init(frame);
}

} // widget
} // boden
