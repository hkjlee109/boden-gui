#include "grid_layer.hpp"

namespace miro {
namespace widget {
namespace layer {

std::shared_ptr<grid_layer_t> grid_layer_t::alloc()
{
    auto instance = std::make_shared<grid_layer_t>();
    instance->init();
    return instance;
}

std::shared_ptr<grid_layer_t> grid_layer_t::alloc(const boden::layout::rect_t &frame)
{
    auto instance = std::make_shared<grid_layer_t>(frame);
    instance->init(frame);
    return instance;
}

grid_layer_t::grid_layer_t()
    : boden::widget::layer::layer_t{},
      _zoom{100}
{
}

grid_layer_t::grid_layer_t(const boden::layout::rect_t &frame)
    : boden::widget::layer::layer_t{frame},
      _zoom{100}
{
}

grid_layer_t::~grid_layer_t()
{
}

void grid_layer_t::draw(boden::builder_t &builder, const boden::layout::rect_t &parent_frame_in_window)
{
    if(_tid == 0)
    {
        return;
    }

    auto frame_in_window = boden::layout::rect_t{parent_frame_in_window.origin + _frame.origin, 
                                                 parent_frame_in_window.size};
    
    if(!_needs_display)
    {
        builder.begin(_tid, frame_in_window, _frame);         
        builder.end();
        return;
    }

    builder.begin(_tid, frame_in_window, _frame, boden::graphic::compositing_operation_t::clear);
    builder.end();
    
    builder.begin(_tid, frame_in_window, _frame, boden::graphic::compositing_operation_t::custom_1);
    std::size_t offset = 0;
    builder.add_param(offset, &_zoom, sizeof(_zoom));
    offset += sizeof(_zoom);
    builder.add_param(offset, &_offset.x, sizeof(_offset.x));
    offset += sizeof(_offset.x);
    builder.add_param(offset, &_offset.y, sizeof(_offset.y));
    builder.end();
    
    _needs_display = false;
}

void grid_layer_t::set_offset(const boden::layout::point_t &offset)
{
    _offset = offset;
    _needs_display = true;
}

void grid_layer_t::set_zoom(uint32_t zoom)
{
    _zoom = zoom;
    _needs_display = true;
}

void grid_layer_t::init()
{
}

void grid_layer_t::init(const boden::layout::rect_t &frame)
{
    set_frame(frame);
}

} // layer
} // widget
} // miro
