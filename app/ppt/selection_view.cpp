#include "selection_view.hpp"

namespace ppt {

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
    if(layer.tid == 0)
    {
        return;
    }
    
    boden::layout::rect_t frame_in_window = convert_rect_to_view(_bounds, nullptr);

    builder.begin(layer.tid, frame_in_window, dirty_rect);

    for(auto &frame_ : _selection_frames)
    {
        builder.push_clip_rect(frame_.inset_by(-HANDLE_SIZE, -HANDLE_SIZE));
        
        boden::layout::point_t pts[] =
        {
            {frame_.origin.x, frame_.origin.y},
            {frame_.origin.x + frame_.size.width, frame_.origin.y},
            {frame_.origin.x, frame_.origin.y + frame_.size.height},
            {frame_.origin.x + frame_.size.width, frame_.origin.y + frame_.size.height},
        
            {frame_.origin.x + frame_.size.width / 2, frame_.origin.y},
            {frame_.origin.x, frame_.origin.y + frame_.size.height / 2},
            {frame_.origin.x + frame_.size.width, frame_.origin.y + frame_.size.height / 2},
            {frame_.origin.x + frame_.size.width / 2, frame_.origin.y + frame_.size.height}
        };
        
        for(const auto &pt : pts)
        {
            builder.add_rect_filled({pt.x - HANDLE_SIZE_HALF, pt.y - HANDLE_SIZE_HALF},
                                    {pt.x + HANDLE_SIZE_HALF, pt.y + HANDLE_SIZE_HALF},
                                    {0xFF, 0xFF, 0xFF, 0xFF});
            builder.add_rect({pt.x - HANDLE_SIZE_HALF, pt.y - HANDLE_SIZE_HALF},
                             {pt.x + HANDLE_SIZE_HALF, pt.y + HANDLE_SIZE_HALF},
                             {0x56, 0x56, 0x56, 0xFF},
                             1);
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

} // boden
