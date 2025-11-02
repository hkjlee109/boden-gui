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
}

void selection_view_t::draw_rect(boden::builder_t &builder, const boden::layout::rect_t &dirty_rect)
{
    std::shared_ptr<boden::widget::layer::layer_t> layer = get_layer();
    if(layer->get_texture_id() == 0)
    {
        return;
    }
    
    boden::layout::rect_t frame_in_window = convert_rect_to_view(_bounds, nullptr);

    builder.begin(layer->get_texture_id(), frame_in_window, dirty_rect);

    for(auto &frame_ : _selection_frames)
    {
        auto frame = frame_.inset_by(1, 1);
        builder.push_clip_rect(frame.inset_by(-HANDLE_SIZE, -HANDLE_SIZE));
        
        boden::layout::point_t pts[] =
        {
            {frame.origin.x, frame.origin.y},
            {frame.origin.x + frame.size.width, frame.origin.y},
            {frame.origin.x, frame.origin.y + frame.size.height},
            {frame.origin.x + frame.size.width, frame.origin.y + frame.size.height},
        
            {frame.origin.x + frame.size.width / 2, frame.origin.y},
            {frame.origin.x, frame.origin.y + frame.size.height / 2},
            {frame.origin.x + frame.size.width, frame.origin.y + frame.size.height / 2},
            {frame.origin.x + frame.size.width / 2, frame.origin.y + frame.size.height}
        };
        
        for(const auto &pt : pts)
        {
            builder.add_rect_filled({pt.x - HANDLE_SIZE_HALF, pt.y - HANDLE_SIZE_HALF},
                                    {pt.x + HANDLE_SIZE_HALF, pt.y + HANDLE_SIZE_HALF},
                                    {0xFF, 0xFF, 0xFF, 0xFF});
        }
        
        builder.pop_clip_rect();
    }
    builder.end();
}

std::shared_ptr<boden::widget::view_t> selection_view_t::hit_test(boden::layout::point_t point)
{
    return nullptr;
}

void selection_view_t::set_selection_frames(std::vector<boden::layout::rect_t> &&frames)
{
    _selection_frames = std::move(frames); 
    set_needs_display(true);
}

void selection_view_t::init(const boden::layout::rect_t &frame)
{
    boden::widget::view_t::init(frame);
}

} // boden
