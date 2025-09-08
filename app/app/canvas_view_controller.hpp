#pragma once

#include <boden/context.hpp>
#include <boden/layout/rect.hpp>
#include <boden/widget/shape/shape.hpp>
#include <boden/widget/view_controller.hpp>
#include <memory>
#include <unordered_set>

namespace app {

class canvas_view_controller_t : public boden::widget::view_controller_t
{
public:
    canvas_view_controller_t();
    canvas_view_controller_t(const boden::layout::rect_t &frame);
    ~canvas_view_controller_t();
    
    void mouse_down(const boden::event_t &ev) override;
    void mouse_dragged(const boden::event_t &ev) override;
    void mouse_up(const boden::event_t &ev) override;
    
    void draw(boden::context_t &ctx);
    
private:
    std::vector<std::shared_ptr<boden::widget::shape::shape_t>> _shapes;
    std::unordered_set<boden::widget::shape::shape_t *> _selection;
    
    boden::layout::point_t _mouse_location_cache;
    
    void init();
};

} // app
