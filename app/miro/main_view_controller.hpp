#pragma once

#include <boden/builder.hpp>
#include <boden/layout/rect.hpp>
#include <boden/widget/view_controller.hpp>
#include <memory>
#include <miro/canvas_view_controller.hpp>
#include <miro/tool_type.hpp>
#include <miro/toolbox_view_controller.hpp>
#include <miro/zoom_view_controller.hpp>

namespace miro {

class main_view_controller_t : public boden::widget::view_controller_t,
                               public miro::toolbox_delegate_t,
                               public miro::zoom_delegate_t
{
public:
    main_view_controller_t();
    ~main_view_controller_t();
    
    void load_view() override;
    void view_did_load() override;
    
    void did_tool_select(boden::widget::view_controller_ref_t sender,
                         miro::tool_type_t type) override;
    void did_zoom_change(boden::widget::view_controller_ref_t sender,
                         uint32_t zoom) override;
    
private:
    std::shared_ptr<miro::canvas_view_controller_t> _canvas_ctrl;
    std::shared_ptr<miro::toolbox_view_controller_t> _toolbox_ctrl;
    std::shared_ptr<miro::zoom_view_controller_t> _zoom_ctrl;
};

} // miro
