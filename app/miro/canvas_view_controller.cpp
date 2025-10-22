#include "canvas_view_controller.hpp"

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
    printf("# did_canvas_view_mouse_down\n");
}

void canvas_view_controller_t::did_canvas_view_mouse_dragged(const boden::layout::point_t &location)
{
}

void canvas_view_controller_t::did_canvas_view_mouse_up(const boden::layout::point_t &location)
{
}

void canvas_view_controller_t::did_canvas_view_scroll_wheel(const boden::layout::vec2_t &delta)
{
    printf("# did_canvas_view_scroll_wheel %f %f\n", delta.x, delta.y);
}

void canvas_view_controller_t::load_view()
{
    auto document_view{std::make_shared<boden::widget::view_t>(boden::layout::rect_t(0, 0, 8192, 4096))};
    auto canvas_view{std::make_shared<miro::canvas_view_t>(boden::layout::rect_t(0, 0, 640, 480))};

    canvas_view->set_delegate(this);
    canvas_view->set_document_view(document_view);
    _view = canvas_view;
}

} // miro
