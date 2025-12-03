#include "tool_button.hpp"

#include <miro/theme/color.hpp>

namespace miro {
namespace widget {

std::shared_ptr<tool_button_t> tool_button_t::alloc(const boden::layout::rect_t &frame)
{
    auto instance = std::make_shared<tool_button_t>(frame);
    instance->init(frame);
    return instance;
}

tool_button_t::tool_button_t()
    : miro::widget::hover_button_t()
{
}

tool_button_t::tool_button_t(const boden::layout::rect_t &frame)
    : miro::widget::hover_button_t(frame),
      _selected{false}
{
}

tool_button_t::~tool_button_t()
{
}

bool tool_button_t::is_selected() const
{
    return _selected;
}
    
void tool_button_t::set_selected(bool selected)
{
    _selected = selected;
    set_content_tint_color(_selected ? miro::theme::color::secondary
                                     : miro::theme::color::control_accent);
}

void tool_button_t::init(const boden::layout::rect_t &frame)
{
    miro::widget::hover_button_t::init(frame);
    _layer->set_background_color({0x00, 0xFF, 0x00, 0xFF});
}

} // widget
} // miro
