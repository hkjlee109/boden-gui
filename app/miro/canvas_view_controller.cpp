#include "canvas_view_controller.hpp"

#include <algorithm>

namespace miro {

canvas_view_controller_t::canvas_view_controller_t()
    : boden::widget::view_controller_t()
{
}

canvas_view_controller_t::~canvas_view_controller_t()
{
}

void canvas_view_controller_t::did_canvas_view_add_subview(const boden::widget::view_t *view)
{
}

void canvas_view_controller_t::did_canvas_view_key_down(uint32_t key_code, uint64_t modifier_flags)
{
}

void canvas_view_controller_t::did_canvas_view_key_up(uint32_t key_code, uint64_t modifier_flags)
{
}

void canvas_view_controller_t::did_canvas_view_mouse_down(const boden::layout::point_t &location)
{
}

void canvas_view_controller_t::did_canvas_view_mouse_dragged(const boden::layout::point_t &location)
{
}

void canvas_view_controller_t::did_canvas_view_mouse_up(const boden::layout::point_t &location)
{
}

void canvas_view_controller_t::did_canvas_view_scroll_wheel(const boden::layout::vec2_t &delta)
{
    auto canvas_view = std::dynamic_pointer_cast<miro::canvas_view_t>(_view);
    _offset.x = std::clamp(_offset.x - delta.x, 0.0f, 4096.0f);
    _offset.y = std::clamp(_offset.y - delta.y, 0.0f, 4096.0f);
    canvas_view->set_offset(_offset);
}

void canvas_view_controller_t::load_view()
{
    auto document_view{boden::widget::view_t::alloc({0, 0, 4096, 4096})};
    auto canvas_view{miro::canvas_view_t::alloc({0, 0, 640, 480})};

    canvas_view->set_delegate(this);
    canvas_view->set_document_view(document_view);
    _view = canvas_view;
}

void canvas_view_controller_t::set_zoom(uint32_t zoom)
{
    auto canvas_view = std::dynamic_pointer_cast<miro::canvas_view_t>(_view);
    canvas_view->set_zoom(zoom);
}

} // miro
