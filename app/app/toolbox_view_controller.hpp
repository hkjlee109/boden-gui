#pragma once

#include <app/components/hover_button.hpp>
#include <boden/layout/rect.hpp>
#include <boden/widget/image_view.hpp>
#include <boden/widget/view_controller.hpp>
#include <memory>

namespace app {

class toolbox_view_controller_t : public boden::widget::view_controller_t
{
public:
    toolbox_view_controller_t();
    toolbox_view_controller_t(const boden::layout::rect_t &frame);
    ~toolbox_view_controller_t();
    
    void load_view() override;
    
    void draw(boden::builder_t &builder);
    void on_button_click(void *sender);
    
private:
    std::shared_ptr<app::components::hover_button_t> _rectangle;
    std::shared_ptr<boden::widget::image_view_t> _image_view;
    
    void init();
};

} // app
