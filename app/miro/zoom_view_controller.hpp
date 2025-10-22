#pragma once

#include <boden/layout/rect.hpp>
#include <boden/widget/view_controller.hpp>
#include <memory>
#include <miro/widget/hover_button.hpp>

namespace miro {

class zoom_view_controller_t : public boden::widget::view_controller_t
{
public:
    zoom_view_controller_t();
    zoom_view_controller_t(const boden::layout::rect_t &frame);
    ~zoom_view_controller_t();
    
    void load_view() override;
    void view_did_load() override;

private:
    std::shared_ptr<miro::widget::hover_button_t> _zoom_in;
    std::shared_ptr<miro::widget::hover_button_t> _zoom_out;
    
    void init();
};

} // miro
