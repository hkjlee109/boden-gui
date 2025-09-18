#pragma once

#include <boden/layout/rect.hpp>
#include <boden/widget/shape/shape.hpp>
#include <boden/widget/view.hpp>
#include <boden/widget/view_controller.hpp>
#include <memory>
#include <unordered_set>

namespace app {

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

    void draw(boden::builder_t &builder);
    
private:
    std::vector<std::shared_ptr<boden::widget::shape::shape_t>> _shapes;
    std::unordered_set<boden::widget::shape::shape_t *> _selection;
    
    boden::layout::point_t _mouse_location_cache;
    
    void init();
};

} // app
