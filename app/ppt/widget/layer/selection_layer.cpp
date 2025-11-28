#include "selection_layer.hpp"

namespace ppt {
namespace widget {
namespace layer {

static constexpr float HANDLE_SIZE = 8.0f;
static constexpr float HANDLE_SIZE_HALF = HANDLE_SIZE / 2;

std::shared_ptr<selection_layer_t> selection_layer_t::alloc()
{
    auto instance = std::make_shared<selection_layer_t>();
    instance->init();
    return instance;
}

std::shared_ptr<selection_layer_t> selection_layer_t::alloc(const boden::layout::rect_t &frame)
{
    auto instance = std::make_shared<selection_layer_t>(frame);
    instance->init(frame);
    return instance;
}

selection_layer_t::selection_layer_t()
    : boden::widget::layer::layer_t{}
{
}

selection_layer_t::selection_layer_t(const boden::layout::rect_t &frame)
    : boden::widget::layer::layer_t{frame}
{
}

selection_layer_t::~selection_layer_t()
{
}

void selection_layer_t::draw(boden::builder_t &builder, const boden::layout::rect_t &parent_frame_in_window)
{
    if(_tid == 0)
    {
        return;
    }

    auto frame_in_window = boden::layout::rect_t{parent_frame_in_window.origin + _frame.origin, 
                                                 parent_frame_in_window.size} * _contents_scale;
    auto frame = _frame * _contents_scale;

    if(!_needs_display)
    {
        builder.begin(_tid, frame_in_window, frame);         
        builder.end();
        return;
    }

    builder.begin(_tid, frame_in_window, frame, boden::graphic::compositing_operation_t::clear);
    builder.end();
    
    float handle_size = HANDLE_SIZE * _contents_scale;
    float handle_size_half = HANDLE_SIZE_HALF * _contents_scale;
    
    builder.begin(_tid, frame_in_window, frame);
    for(auto &frame_ : _selection_frames)
    {
        auto frame = frame_.inset_by(1, 1) * _contents_scale;
        builder.push_clip_rect(frame.inset_by(-handle_size, -handle_size));
    
        boden::layout::point_t pts[] =
        {
            {frame.origin.x,                    frame.origin.y},
            {frame.origin.x + frame.size.width, frame.origin.y},
            {frame.origin.x,                    frame.origin.y + frame.size.height},
            {frame.origin.x + frame.size.width, frame.origin.y + frame.size.height},
    
            {frame.origin.x + frame.size.width / 2, frame.origin.y},
            {frame.origin.x,                        frame.origin.y + frame.size.height / 2},
            {frame.origin.x + frame.size.width,     frame.origin.y + frame.size.height / 2},
            {frame.origin.x + frame.size.width / 2, frame.origin.y + frame.size.height}
        };
    
        for(const auto &pt : pts)
        {
            builder.add_rect_filled({pt.x - handle_size_half, pt.y - handle_size_half},
                                    {pt.x + handle_size_half, pt.y + handle_size_half},
                                    {0xFF, 0xFF, 0xFF, 0xFF});
        }

        builder.pop_clip_rect();
    }
    builder.end();
    
    _needs_display = false;
}

void selection_layer_t::set_selection_frames(std::vector<boden::layout::rect_t> &&frames)
{
    _selection_frames = std::move(frames);
    _needs_display = true;
}

void selection_layer_t::set_offset(const boden::layout::point_t &offset)
{
    _needs_display = true;
}

void selection_layer_t::set_zoom(uint32_t zoom)
{
    _needs_display = true;
}

void selection_layer_t::init()
{
}

void selection_layer_t::init(const boden::layout::rect_t &frame)
{
    set_frame(frame);
}

} // layer
} // widget
} // ppt
