#include "canvas_view.hpp"

namespace ppt {

canvas_view_t::canvas_view_t()
    : boden::widget::view_t{},
      _delegate{nullptr}
{
}

canvas_view_t::canvas_view_t(const boden::layout::rect_t &frame)
    : boden::widget::view_t{frame},
      _delegate{nullptr}
{
}

canvas_view_t::~canvas_view_t()
{
}

bool canvas_view_t::accepts_first_responder()
{
    return true;
}

void canvas_view_t::did_add_subview(const boden::widget::view_t *view)
{
    if(_delegate) 
    {
        _delegate->did_canvas_view_add_subview(view);
    }
}

std::shared_ptr<boden::widget::view_t> canvas_view_t::hit_test(boden::layout::point_t point)
{
    if(_hidden) 
    {
        return nullptr;
    }

    boden::layout::point_t origin = convert_point_to_view({0, 0}, nullptr);
    boden::layout::rect_t frame{origin, _frame.size};

    if(!frame.contains(point)) 
    {
        return nullptr;
    }

    return shared_from_this();
}

void canvas_view_t::key_down(const boden::event_t &event)
{
    if(_delegate)
    {
        uint64_t flags = (static_cast<uint64_t>(event.modifier_flags_msb) << 32) |
                          static_cast<uint64_t>(event.modifier_flags_lsb);
        _delegate->did_canvas_view_key_down(event.key_code, flags);
    }
}

void canvas_view_t::key_up(const boden::event_t &event)
{
    if(_delegate)
    {
        uint64_t flags = (static_cast<uint64_t>(event.modifier_flags_msb) << 32) |
                          static_cast<uint64_t>(event.modifier_flags_lsb);
        _delegate->did_canvas_view_key_up(event.key_code, flags);
    }
}

void canvas_view_t::mouse_down(const boden::event_t &event)
{
    if(_delegate) 
    {
        _delegate->did_canvas_view_mouse_down(event.location);
    }
}

void canvas_view_t::mouse_dragged(const boden::event_t &event)
{
    if(_delegate)
    {
        _delegate->did_canvas_view_mouse_dragged(event.location);
    }
}

void canvas_view_t::mouse_up(const boden::event_t &event)
{
    if(_delegate)
    {
        _delegate->did_canvas_view_mouse_up(event.location);
    }
}

void canvas_view_t::set_delegate(ppt::canvas_view_delegate_t *delegate)
{
    _delegate = delegate;
}

} // boden
