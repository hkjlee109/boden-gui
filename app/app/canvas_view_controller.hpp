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
    
    void draw(boden::context_t &ctx);
    
    void on_background_mouse_up(void *sender);
    void on_shape_mouse_down(void *sender);

private:
    std::vector<std::shared_ptr<boden::widget::shape::shape_t>> _shapes;
    std::unordered_set<boden::widget::shape::shape_t *> _selection;
    
    void init();
};

} // app
