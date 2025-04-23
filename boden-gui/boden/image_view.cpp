#include "image_view.hpp"
#include <boden/renderer.hpp>

namespace boden {

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
    
    builder.push_clip_rect({_frame.origin.x, _frame.origin.y, _frame.size.width, _frame.size.height});
    
    builder.add_image(_image->texture_id, 
                      {_frame.origin.x, _frame.origin.y}, 
                      {_frame.origin.x + _frame.size.width, _frame.origin.y + _frame.size.height},
                      _tint_color);

    builder.pop_clip_rect();
}

void image_view_t::set_image(std::unique_ptr<boden::image_t> image)
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

} // boden
