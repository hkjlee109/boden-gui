#pragma once

#include <boden/layout/rect.hpp>
#include <boden/widget/image_view.hpp>
#include <boden/widget/view_controller.hpp>
#include <memory>
#include <ppt/widget/hover_button.hpp>
#include <ppt/shape_type.hpp>

namespace ppt {

class toolbox_view_controller_t;

class toolbox_delegate_t {
public:
    virtual void did_toolbox_select(std::shared_ptr<boden::widget::view_controller_t> sender,
                                    ppt::shape_type_t shape_type) = 0;

    virtual ~toolbox_delegate_t() = default;
};

class toolbox_view_controller_t : public boden::widget::view_controller_t
{
public:
    toolbox_view_controller_t();
    toolbox_view_controller_t(const boden::layout::rect_t &frame);
    ~toolbox_view_controller_t();
    
    void load_view() override;
    
    void set_toolbox_delegate(ppt::toolbox_delegate_t *delegate);
    
    void draw(boden::builder_t &builder);
    void on_tool_button_click(void *sender);
    
private:
    ppt::toolbox_delegate_t *_toolbox_delegate;
    
    std::shared_ptr<ppt::widget::hover_button_t> _rectangle;
    std::shared_ptr<ppt::widget::hover_button_t> _textbox;
    std::shared_ptr<boden::widget::image_view_t> _image_view;
    
    void init();
};

} // ppt
