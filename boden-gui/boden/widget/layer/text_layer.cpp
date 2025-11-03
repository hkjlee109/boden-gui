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
    : boden::widget::layer::layer_t{}
{
}

text_layer_t::text_layer_t(const boden::layout::rect_t &frame)
    : boden::widget::layer::layer_t{frame}
{
}

text_layer_t::~text_layer_t()
{
}

void text_layer_t::init()
{
}

void text_layer_t::init(const boden::layout::rect_t &frame)
{
    set_frame(frame);
}

} // layer
} // widget
} // boden
