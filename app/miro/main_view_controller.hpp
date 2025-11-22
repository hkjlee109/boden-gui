#pragma once

#include <boden/builder.hpp>
#include <boden/layout/rect.hpp>
#include <boden/widget/view_controller.hpp>
#include <memory>
#include <miro/canvas_view_controller.hpp>
#include <miro/zoom_view_controller.hpp>

namespace miro {

class main_view_controller_t : public boden::widget::view_controller_t,
                               public miro::zoom_delegate_t
{
public:
    main_view_controller_t();
    ~main_view_controller_t();
    
    void load_view() override;
    
    void did_zoom_change(uint32_t zoom) override;
    
private:
    std::shared_ptr<miro::canvas_view_controller_t> _canvas_ctrl;
    std::shared_ptr<miro::zoom_view_controller_t> _zoom_ctrl;
};

} // miro
