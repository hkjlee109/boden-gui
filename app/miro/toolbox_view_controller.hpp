#pragma once

#include <boden/layout/rect.hpp>
#include <boden/widget/view_controller.hpp>
#include <memory>
#include <miro/tool_type.hpp>
#include <miro/widget/hover_button.hpp>

namespace miro {

class toolbox_view_controller_t;

class toolbox_delegate_t {
public:
    virtual void did_tool_select(boden::widget::view_controller_ref_t sender,
                                 miro::tool_type_t type) = 0;

    virtual ~toolbox_delegate_t() = default;
};

class toolbox_view_controller_t : public boden::widget::view_controller_t
{
public:
    toolbox_view_controller_t();
    toolbox_view_controller_t(const boden::layout::rect_t &frame);
    ~toolbox_view_controller_t();
    
    void load_view() override;
    void view_did_load() override;
    
    void set_toolbox_delegate(miro::toolbox_delegate_t *delegate);
    
    void on_selector_click(void *sender);
    
private:
    miro::toolbox_delegate_t *_toolbox_delegate;
    
    std::shared_ptr<miro::widget::hover_button_t> _selector;
    
    void init();
};

} // miro
