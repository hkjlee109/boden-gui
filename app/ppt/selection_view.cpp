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

void selection_view_t::draw(boden::builder_t &builder)
{
    boden::layout::point_t origin = convert_point_to_view({0, 0}, nullptr);

    for(auto &frame_ : _selection_frames)
    {
        boden::layout::rect_t frame{origin.x + frame_.origin.x,
                                    origin.y + frame_.origin.y,
                                    frame_.size.width,
                                    frame_.size.height};
        
        builder.push_clip_rect({frame.origin.x - HANDLE_SIZE_HALF - 1,
                                frame.origin.y - HANDLE_SIZE_HALF - 1,
                                frame.size.width + HANDLE_SIZE + 2,
                                frame.size.height + HANDLE_SIZE + 2});
        
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
            builder.add_rect({pt.x - HANDLE_SIZE_HALF, pt.y - HANDLE_SIZE_HALF},
                             {pt.x + HANDLE_SIZE_HALF, pt.y + HANDLE_SIZE_HALF},
                             {0x56, 0x56, 0x56, 0xFF},
                             1);
        }
        
        builder.pop_clip_rect();
    }
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
