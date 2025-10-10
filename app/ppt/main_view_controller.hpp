#pragma once

#include <boden/builder.hpp>
#include <boden/layout/rect.hpp>
#include <boden/widget/view_controller.hpp>
#include <memory>
#include <ppt/canvas_view_controller.hpp>
#include <ppt/toolbox_view_controller.hpp>

namespace ppt {

class main_view_controller_t : public boden::widget::view_controller_t,
                               public ppt::toolbox_delegate_t
{
public:
    main_view_controller_t();
    main_view_controller_t(const boden::layout::rect_t &frame);
    ~main_view_controller_t();
    
    void load_view() override;
    
    void did_toolbox_select(std::shared_ptr<boden::widget::view_controller_t> sender,
                            ppt::shape_type_t shape_type) override;
    
private:
    std::shared_ptr<ppt::canvas_view_controller_t> _canvas_ctrl;
    std::shared_ptr<ppt::toolbox_view_controller_t> _toolbox_ctrl;
};

} // ppt
