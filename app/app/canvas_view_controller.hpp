#pragma once

#include <boden/context.hpp>
#include <boden/layout/rect.hpp>
#include <boden/widget/shape/rectangle.hpp>
#include <boden/widget/view_controller.hpp>
#include <memory>

namespace app {

class canvas_view_controller_t : public boden::widget::view_controller_t
{
public:
    canvas_view_controller_t();
    canvas_view_controller_t(const boden::layout::rect_t &frame);
    ~canvas_view_controller_t();
    
    void draw(boden::context_t &ctx);
    
private:
    std::shared_ptr<boden::widget::shape::rectangle_t> _rectangle;
    
    void init();
};

} // app
