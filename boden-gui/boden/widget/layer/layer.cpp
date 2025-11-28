#include "layer.hpp"

namespace boden {
namespace widget {
namespace layer {

std::shared_ptr<layer_t> layer_t::alloc()
{
    auto instance = std::make_shared<layer_t>();
    instance->init();
    return instance;
}

std::shared_ptr<layer_t> layer_t::alloc(const boden::layout::rect_t &frame)
{    
    auto instance = std::make_shared<layer_t>(frame);
    instance->init(frame);
    return instance;
}

layer_t::layer_t()
    : _bounds{0, 0, 0, 0},
      _frame{0, 0, 0, 0},
      _background_color{},
      _border_color{},
      _border_width{0},
      _contents_scale{1},
      _corner_radius{0},
      _needs_display{false},
      _tid{0}
{
}

layer_t::layer_t(const boden::layout::rect_t &frame)
    : _bounds{0, 0, frame.size.width, frame.size.height},
      _frame{frame},
      _background_color{},
      _border_color{},
      _border_width{0},
      _contents_scale{1},
      _corner_radius{0},
      _needs_display{false},
      _tid{0}
{
}

layer_t::~layer_t()
{
}

void layer_t::draw(boden::builder_t &builder, const boden::layout::rect_t &parent_frame_in_window)
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

    builder.begin(_tid, frame_in_window, _frame);
    builder.add_rect_filled({_bounds.min_x(), _bounds.min_y()}, 
                            {_bounds.max_x(), _bounds.max_y()},
                            _background_color, 
                            _corner_radius);

    if(_border_width > 0)
    {
        builder.add_rect({_bounds.min_x(), _bounds.min_y()}, 
                         {_bounds.max_x(), _bounds.max_y()},
                         _border_color,
                         _border_width);
    }
    builder.end();

    _needs_display = false;
}

const boden::layout::color_t & layer_t::get_background_color() const 
{ 
    return _background_color; 
}

void layer_t::set_background_color(const boden::layout::color_t &color) 
{ 
    _background_color = color;
    _needs_display = true;
}

const boden::layout::color_t & layer_t::get_border_color() const 
{ 
    return _border_color; 
}

void layer_t::set_border_color(const boden::layout::color_t &color) 
{ 
    _border_color = color;
    _needs_display = true;
}

float layer_t::get_border_width() const 
{ 
    return _border_width; 
}

void layer_t::set_border_width(float width) 
{ 
    _border_width = width; 
    _needs_display = true;
}

float layer_t::get_contents_scale() const 
{ 
    return _contents_scale; 
}

void layer_t::set_contents_scale(float scale) 
{ 
    _contents_scale = scale;

    for(const auto &layer : _sublayers) 
    {
        layer->set_contents_scale(scale);
    }

    _needs_display = true;
}

float layer_t::get_corner_radius() const 
{ 
    return _corner_radius; 
}

void layer_t::set_corner_radius(float radius) 
{ 
    _corner_radius = radius; 
    _needs_display = true;
}

const boden::layout::rect_t & layer_t::get_frame() const
{
    return _frame;
}

void layer_t::set_frame(const boden::layout::rect_t &frame)
{
    _frame = frame;
    _bounds = {0, 0, frame.size.width, frame.size.height};
    
    _needs_display = true;
}

boden::graphic::texture_id_t layer_t::get_texture_id() const 
{ 
    return _tid; 
}

bool layer_t::get_needs_display() const
{
    return _needs_display;
}

void layer_t::set_needs_display(bool needs)
{
    _needs_display = needs;
}

void layer_t::set_texture_id(boden::graphic::texture_id_t tid) 
{ 
    _tid = tid;
    _needs_display = true;
}

const std::vector<std::shared_ptr<boden::widget::layer::layer_t>> & layer_t::get_sublayers() const
{
    return _sublayers;
}

void layer_t::add_layer(std::shared_ptr<boden::widget::layer::layer_t> layer)
{
    layer->set_contents_scale(_contents_scale);
    _sublayers.push_back(layer);
}

void layer_t::remove_layer(std::shared_ptr<boden::widget::layer::layer_t> layer)
{
    _sublayers.erase(std::remove_if(_sublayers.begin(), 
                                    _sublayers.end(),
                                    [layer](const std::shared_ptr<boden::widget::layer::layer_t> &child) 
                                    {
                                        return child.get() == layer.get();
                                    }), 
                                    _sublayers.end());
}

void layer_t::init()
{
}

void layer_t::init(const boden::layout::rect_t &frame)
{
    set_frame(frame);
}

} // layer
} // widget
} // boden
