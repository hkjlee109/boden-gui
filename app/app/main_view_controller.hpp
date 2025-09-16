#pragma once

#include <boden/builder.hpp>
#include <boden/layout/rect.hpp>
#include <boden/widget/view_controller.hpp>
#include <memory>

namespace app {

class main_view_controller_t : public boden::widget::view_controller_t
{
public:
    main_view_controller_t();
    main_view_controller_t(const boden::layout::rect_t &frame);
    ~main_view_controller_t();
    
    void load_view() override;
    void mouse_down(const boden::event_t &ev) override;
    void mouse_dragged(const boden::event_t &ev) override;
    void mouse_up(const boden::event_t &ev) override;


    void draw(boden::builder_t &builder);
    
    void on_button_click(void *sender);

private:
};

} // app
