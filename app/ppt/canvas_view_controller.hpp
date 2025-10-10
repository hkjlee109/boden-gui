#pragma once

#include <boden/layout/rect.hpp>
#include <boden/widget/view.hpp>
#include <boden/widget/view_controller.hpp>
#include <memory>
#include <ppt/shape_type.hpp>
#include <ppt/widget/shape/shape.hpp>
#include <unordered_set>

namespace ppt {

class canvas_view_controller_t : public boden::widget::view_controller_t,
                                 public boden::widget::view_delegate_t
{
public:
    canvas_view_controller_t();
    canvas_view_controller_t(const boden::layout::rect_t &frame);
    ~canvas_view_controller_t();
    
    void load_view() override;
    
    void did_view_mouse_down(std::shared_ptr<boden::widget::view_t> sender,
                             const boden::layout::point_t &location) override;

    void did_view_mouse_dragged(std::shared_ptr<boden::widget::view_t> sender,
                                const boden::layout::point_t &location) override;

    void did_view_mouse_up(std::shared_ptr<boden::widget::view_t> sender,
                           const boden::layout::point_t &location) override;

    void create_shape(ppt::shape_type_t type);
    
private:
    std::vector<std::shared_ptr<ppt::widget::shape::shape_t>> _shapes;
    std::unordered_set<ppt::widget::shape::shape_t *> _selection;
    
    boden::layout::point_t _mouse_location_cache;
    
    void init();
    void clear_selection();
};

} // ppt
