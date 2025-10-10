#include "image_view.hpp"
#include <boden/renderer.hpp>

namespace boden {
namespace widget {

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

void image_view_t::draw(boden::builder_t &builder)
{
    if(_hidden) 
    {
        return;
    }
    
    boden::layout::point_t origin = convert_point_to_view({0, 0}, nullptr);
    boden::layout::rect_t frame{origin, _frame.size};

    builder.push_clip_rect({frame.origin.x, frame.origin.y, frame.size.width, frame.size.height});
    
    builder.add_image(_image->key, 
                      {frame.origin.x, frame.origin.y}, 
                      {frame.origin.x + frame.size.width, frame.origin.y + frame.size.height},
                      _tint_color);

    builder.pop_clip_rect();
}

void image_view_t::set_image(std::unique_ptr<boden::widget::base::image_t> image)
{
    _image = std::move(image);
}

void image_view_t::set_tint_color(const boden::layout::color_t &color)
{
    _tint_color = color;
}

const boden::layout::color_t & image_view_t::get_tint_color() const
{
    return _tint_color;
}

} // widget
} // boden
