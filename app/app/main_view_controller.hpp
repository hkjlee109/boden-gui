#pragma once

#include <app/canvas_view_controller.hpp>
#include <app/toolbox_view_controller.hpp>
#include <boden/builder.hpp>
#include <boden/layout/rect.hpp>
#include <boden/widget/view_controller.hpp>
#include <memory>

namespace app {

class main_view_controller_t : public boden::widget::view_controller_t,
                               public app::toolbox_delegate_t
{
public:
    main_view_controller_t();
    main_view_controller_t(const boden::layout::rect_t &frame);
    ~main_view_controller_t();
    
    void load_view() override;
    
    void did_toolbox_select(std::shared_ptr<boden::widget::view_controller_t> sender,
                            app::shape_type_t shape_type) override;
    
private:
    std::shared_ptr<app::canvas_view_controller_t> _canvas_ctrl;
    std::shared_ptr<app::toolbox_view_controller_t> _toolbox_ctrl;
};

} // app
