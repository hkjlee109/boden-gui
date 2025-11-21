#include "canvas_view.hpp"

namespace miro {

std::shared_ptr<canvas_view_t> canvas_view_t::alloc(const boden::layout::rect_t &frame)
{
    auto instance = std::make_shared<canvas_view_t>(frame);
    instance->init(frame);
    return instance;
}

canvas_view_t::canvas_view_t()
    : boden::widget::scroll_view_t{},
      _delegate{nullptr}
{
}

canvas_view_t::canvas_view_t(const boden::layout::rect_t &frame)
    : boden::widget::scroll_view_t{frame},
      _delegate{nullptr}
{
}

canvas_view_t::~canvas_view_t()
{
}

void canvas_view_t::draw_rect(boden::builder_t &builder, const boden::layout::rect_t &dirty_rect)
{
    boden::widget::view_t::draw_rect(builder, dirty_rect);

    if(_hidden) 
    {
        return;
    }
    
    auto frame_in_window = convert_rect_to_view(_bounds, nullptr);
    _grid_layer->draw(builder, frame_in_window);
}

void canvas_view_t::view_will_move_to_window(std::shared_ptr<boden::widget::window_t> window)
{
    boden::widget::view_t::view_will_move_to_window(window);
    create_grid_layer_texture();
}

void canvas_view_t::set_frame(const boden::layout::rect_t &frame)
{
    boden::widget::view_t::set_frame(frame);

    if(_grid_layer->get_frame() == frame)
    {
        return;
    }

    _grid_layer->set_frame({0, 0, frame.size.width, frame.size.height});    
    create_grid_layer_texture();
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

void canvas_view_t::scroll_wheel(const boden::event_t &event)
{
    if(_delegate)
    {
        _delegate->did_canvas_view_scroll_wheel({event.scrolling_delta_x,event.scrolling_delta_y});
    }
}

void canvas_view_t::set_delegate(miro::canvas_view_delegate_t *delegate)
{
    _delegate = delegate;
}

void canvas_view_t::init(const boden::layout::rect_t &frame)
{
    boden::widget::scroll_view_t::init(frame);

    _grid_layer = miro::widget::layer::grid_layer_t::alloc({0, 0, frame.size.width, frame.size.height});
    _layer->add_layer(_grid_layer);
}

void canvas_view_t::create_grid_layer_texture()
{
    auto window = _window.lock();
    if(!window)
    {
        return;
    }

    auto tid = _grid_layer->get_texture_id();
    if(tid)
    {
        window->destroy_view_texture(tid);
    }

    _grid_layer->set_texture_id(window->create_view_texture(_grid_layer->get_frame().size));
}

} // miro
