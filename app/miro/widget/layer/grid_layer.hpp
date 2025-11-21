#pragma once

#include <boden/layout/rect.hpp>
#include <boden/widget/layer/layer.hpp>
#include <memory>

namespace miro {
namespace widget {
namespace layer {

class grid_layer_t : public boden::widget::layer::layer_t
{
public:
    static std::shared_ptr<grid_layer_t> alloc();
    static std::shared_ptr<grid_layer_t> alloc(const boden::layout::rect_t &frame);

    grid_layer_t();
    explicit grid_layer_t(const boden::layout::rect_t &frame);
    ~grid_layer_t();
    
    void draw(boden::builder_t &builder, const boden::layout::rect_t &parent_frame_in_window) override;
    
    void set_offset(uint32_t offset_x, uint32_t offset_y);
    void set_zoom(uint32_t zoom);

protected:
    void init() override;
    void init(const boden::layout::rect_t &frame) override;

private:
    uint32_t _offset_x;
    uint32_t _offset_y;
    uint32_t _zoom;
};

} // layer
} // widget
} // miro
