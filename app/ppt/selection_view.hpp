
#pragma once

#include <boden/builder.hpp>
#include <boden/layout/point.hpp>
#include <boden/layout/rect.hpp>
#include <boden/widget/view.hpp>
#include <ppt/widget/layer/selection_layer.hpp>
#include <memory>

namespace ppt {

class selection_view_t : public boden::widget::view_t
{
public:
    static std::shared_ptr<selection_view_t> alloc(const boden::layout::rect_t &frame);

    selection_view_t();
    selection_view_t(const boden::layout::rect_t &frame);
    ~selection_view_t() override;

    void draw_rect(boden::builder_t &builder, const boden::layout::rect_t &dirty_rect) override;
    boden::widget::view_ref_t hit_test(boden::layout::point_t point) override;
    void make_backing_layer() override;
    void view_will_move_to_window(std::shared_ptr<boden::widget::window_t> window) override;
    void set_frame(const boden::layout::rect_t &frame) override;
    
    void set_selection_frames(std::vector<boden::layout::rect_t> &&frames);

protected:
    void init(const boden::layout::rect_t &frame) override;
    
private:
    std::shared_ptr<ppt::widget::layer::selection_layer_t> _selection_layer;
};

} // boden
