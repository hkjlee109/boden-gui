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
      _corner_radius{0},
      _needs_display{true},
      _tid{0}
{
}

layer_t::layer_t(const boden::layout::rect_t &frame)
    : _bounds{0, 0, frame.size.width, frame.size.height},
      _frame{frame},
      _background_color{},
      _border_color{},
      _border_width{0},
      _corner_radius{0},
      _needs_display{true},
      _tid{0}
{
}

layer_t::~layer_t()
{
}

void layer_t::draw(boden::builder_t &builder)
{
}

const boden::layout::color_t & layer_t::get_background_color() const 
{ 
    return _background_color; 
}

void layer_t::set_background_color(const boden::layout::color_t &color) 
{ 
    _background_color = color; 
}

const boden::layout::color_t & layer_t::get_border_color() const 
{ 
    return _border_color; 
}

void layer_t::set_border_color(const boden::layout::color_t &color) 
{ 
    _border_color = color; 
}

float layer_t::get_border_width() const 
{ 
    return _border_width; 
}

void layer_t::set_border_width(float width) 
{ 
    _border_width = width; 
}

float layer_t::get_corner_radius() const 
{ 
    return _corner_radius; 
}

void layer_t::set_corner_radius(float radius) 
{ 
    _corner_radius = radius; 
}

const boden::layout::rect_t & layer_t::get_frame() const
{
    return _frame;
}

void layer_t::set_frame(const boden::layout::rect_t &frame)
{
    _frame = frame;
    _bounds = {0, 0, frame.size.width, frame.size.height};
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
