#pragma once

#include <boden/context.hpp>
#include <boden/layout/rect.hpp>
#include <boden/widget/button.hpp>
#include <boden/widget/image_view.hpp>
#include <boden/widget/shape/rectangle.hpp>
#include <boden/widget/view_controller.hpp>
#include <memory>

namespace app {

class main_view_controller_t : public boden::widget::view_controller_t
{
public:
    main_view_controller_t();
    main_view_controller_t(const boden::layout::rect_t &frame);
    ~main_view_controller_t();
    
    void draw(boden::context_t &ctx);
    
    void on_button_click();
    
private:
    std::shared_ptr<boden::widget::button_t> _button;
    std::shared_ptr<boden::widget::image_view_t> _image_view;
    std::shared_ptr<boden::widget::shape::rectangle_t> _rectangle;
    
    void init();
};

} // app
