#include "grid_layer.hpp"

namespace miro {
namespace widget {
namespace layer {

static constexpr float SPACING = 80.0f;

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
    
    auto bounds = _bounds * _contents_scale;
    auto offset = _offset * _contents_scale;
    float zoom = _zoom / 100.0f;
    float spacing = SPACING * _contents_scale;
    
    float t = std::clamp(1.0f - zoom, 0.0f, 1.0f);
    uint8_t p = (uint8_t)(0xC2 + (0xFF - 0xC2) * t);
    uint8_t s = (uint8_t)(0xF2 - 0x30 * (zoom / 4));
    boden::layout::color_t primary_color{p, p, p, 0xFF };
    boden::layout::color_t secondary_color{s, s, s, 0xFF};

    builder.begin(_tid, frame_in_window, frame, boden::graphic::compositing_operation_t::copy);
    builder.add_grid({bounds.origin.x, bounds.origin.y},
                     {bounds.origin.x + bounds.size.width, bounds.origin.x + bounds.size.height},
                     {offset.x, offset.y},
                     primary_color,
                     secondary_color,
                     spacing,
                     zoom);
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
