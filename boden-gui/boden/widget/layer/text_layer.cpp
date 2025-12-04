#include "text_layer.hpp"

namespace boden {
namespace widget {
namespace layer {

std::shared_ptr<text_layer_t> text_layer_t::alloc()
{
    auto instance = std::make_shared<text_layer_t>();
    instance->init();
    return instance;
}

std::shared_ptr<text_layer_t> text_layer_t::alloc(const boden::layout::rect_t &frame)
{    
    auto instance = std::make_shared<text_layer_t>(frame);
    instance->init(frame);
    return instance;
}

text_layer_t::text_layer_t()
    : boden::widget::layer::layer_t{},
      _text_color{0xFF, 0xFF, 0xFF, 0xFF}
{
}

text_layer_t::text_layer_t(const boden::layout::rect_t &frame)
    : boden::widget::layer::layer_t{frame},
      _text_color{0xFF, 0xFF, 0xFF, 0xFF}
{
}

text_layer_t::~text_layer_t()
{
}

void text_layer_t::draw(boden::builder_t &builder, const boden::layout::rect_t &parent_frame_in_window)
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

    if(_text.empty())
    {
        return;
    }

    auto bounds = _bounds * _contents_scale;

    builder.begin(_tid, frame_in_window, frame, boden::graphic::compositing_operation_t::copy);
    float padding = 5;
    builder.add_text(_text,
                     {bounds.origin.x + padding, bounds.origin.y + padding},
                     {bounds.origin.x + bounds.size.width - padding, bounds.origin.y + bounds.size.height - padding},
                     _text_color,
                     _contents_scale);
    builder.end();
    
    _needs_display = false;
    return;
}

void text_layer_t::init()
{
}

void text_layer_t::init(const boden::layout::rect_t &frame)
{
    set_frame(frame);
}

const std::string & text_layer_t::get_text() const
{
    return _text;
}

void text_layer_t::set_text(const std::string &text)
{
    _text = text;
    _needs_display = true;
}

void text_layer_t::set_text_color(const boden::layout::color_t &color)
{
    _text_color = color;
    _needs_display = true;
}

} // layer
} // widget
} // boden
