#pragma once

#include <boden/layout/rect.hpp>
#include <boden/widget/view.hpp>
#include <boden/widget/view_controller.hpp>
#include <memory>
#include <ppt/canvas_view.hpp>
#include <ppt/selection_view_controller.hpp>
#include <ppt/shape_type.hpp>
#include <ppt/widget/shape/shape.hpp>
#include <unordered_set>

namespace ppt {

class canvas_view_controller_t : public boden::widget::view_controller_t,
                                 public ppt::canvas_view_delegate_t
{
public:
    canvas_view_controller_t();
    canvas_view_controller_t(const boden::layout::rect_t &frame);
    ~canvas_view_controller_t();
    
    void load_view() override;
    void did_canvas_view_add_subview(const boden::widget::view_t *view) override;
    void did_canvas_view_key_down(uint32_t key_code, uint64_t modifier_flags) override;
    void did_canvas_view_key_up(uint32_t key_code, uint64_t modifier_flags) override;
    void did_canvas_view_mouse_down(const boden::layout::point_t &location) override;
    void did_canvas_view_mouse_dragged(const boden::layout::point_t &location) override;
    void did_canvas_view_mouse_up(const boden::layout::point_t &location) override;

    void create_shape(ppt::shape_type_t type);
    
private:
    std::vector<std::shared_ptr<ppt::widget::shape::shape_t>> _shapes;
    std::shared_ptr<ppt::selection_view_controller_t> _selection_ctrl;
    
    boden::layout::point_t _mouse_location_cache;
    
    void init();
};

} // ppt
