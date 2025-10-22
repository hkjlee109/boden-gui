#pragma once

#include <boden/layout/rect.hpp>
#include <boden/widget/image_view.hpp>
#include <boden/widget/view_controller.hpp>
#include <memory>
#include <miro/widget/hover_button.hpp>

namespace miro {

class toolbox_view_controller_t;

class toolbox_delegate_t {
public:
    virtual void did_toolbox_select(std::shared_ptr<boden::widget::view_controller_t> sender) = 0;

    virtual ~toolbox_delegate_t() = default;
};

class toolbox_view_controller_t : public boden::widget::view_controller_t
{
public:
    toolbox_view_controller_t();
    toolbox_view_controller_t(const boden::layout::rect_t &frame);
    ~toolbox_view_controller_t();
    
    void load_view() override;
    
    void set_toolbox_delegate(miro::toolbox_delegate_t *delegate);
    
    void on_tool_button_click(void *sender);
    
private:
    miro::toolbox_delegate_t *_toolbox_delegate;
    
    std::shared_ptr<miro::widget::hover_button_t> _rectangle;
    std::shared_ptr<miro::widget::hover_button_t> _textbox;
    
    void init();
};

} // miro
