#include "selection_view.hpp"

namespace ppt {

std::shared_ptr<selection_view_t> selection_view_t::alloc(const boden::layout::rect_t &frame)
{
    auto instance = std::make_shared<selection_view_t>(frame);
    instance->init(frame);
    return instance;
}

selection_view_t::selection_view_t()
    : boden::widget::view_t{}
{
}

selection_view_t::selection_view_t(const boden::layout::rect_t &frame)
    : boden::widget::view_t{frame}
{
}

selection_view_t::~selection_view_t()
{
    auto tid = _selection_layer->get_texture_id();
    if(tid)
    {
        if(auto window = _window.lock())
        {
            window->destroy_view_texture(tid);
        }
    }
}

void selection_view_t::draw_rect(boden::builder_t &builder, const boden::layout::rect_t &dirty_rect)
{
    if(_hidden)
    {
        return;
    }
    
    auto frame_in_window = convert_rect_to_view(_bounds, nullptr);
    _selection_layer->draw(builder, frame_in_window);
}

boden::widget::view_ref_t selection_view_t::hit_test(boden::layout::point_t point)
{
    return nullptr;
}

void selection_view_t::make_backing_layer()
{
    boden::widget::view_t::make_backing_layer();

    auto window = _window.lock();
    if(!window)
    {
        return;
    }

    auto tid = _selection_layer->get_texture_id();
    if(tid)
    {
        window->destroy_view_texture(tid);
    }

    auto size = _selection_layer->get_frame().size * _selection_layer->get_contents_scale();
    _selection_layer->set_texture_id(window->create_view_texture(size));
}

void selection_view_t::view_will_move_to_window(std::shared_ptr<boden::widget::window_t> window)
{
    boden::widget::view_t::view_will_move_to_window(window);
    make_backing_layer();
}

void selection_view_t::set_frame(const boden::layout::rect_t &frame)
{
    boden::widget::view_t::set_frame(frame);

    if(_selection_layer->get_frame() == frame)
    {
        return;
    }

    _selection_layer->set_frame({0, 0, frame.size.width, frame.size.height});
    make_backing_layer();
}

void selection_view_t::set_selection_frames(std::vector<boden::layout::rect_t> &&frames)
{
    _selection_layer->set_selection_frames(std::move(frames));
}

void selection_view_t::init(const boden::layout::rect_t &frame)
{
    boden::widget::view_t::init(frame);
    
    _selection_layer = ppt::widget::layer::selection_layer_t::alloc({0, 0, frame.size.width, frame.size.height});
    _layer->add_layer(_selection_layer);
}

} // boden
