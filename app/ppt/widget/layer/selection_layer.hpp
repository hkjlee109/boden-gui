#pragma once

#include <boden/layout/rect.hpp>
#include <boden/layout/point.hpp>
#include <boden/widget/layer/layer.hpp>
#include <memory>

namespace ppt {
namespace widget {
namespace layer {

class selection_layer_t : public boden::widget::layer::layer_t
{
public:
    static std::shared_ptr<selection_layer_t> alloc();
    static std::shared_ptr<selection_layer_t> alloc(const boden::layout::rect_t &frame);

    selection_layer_t();
    explicit selection_layer_t(const boden::layout::rect_t &frame);
    ~selection_layer_t();
    
    void draw(boden::builder_t &builder, const boden::layout::rect_t &parent_frame_in_window) override;
    
    void set_selection_frames(std::vector<boden::layout::rect_t> &&frames);
    void set_offset(const boden::layout::point_t &offset);
    void set_zoom(uint32_t zoom);

protected:
    void init() override;
    void init(const boden::layout::rect_t &frame) override;

private:
    std::vector<boden::layout::rect_t> _selection_frames;
};

} // layer
} // widget
} // ppt
