#pragma once

#include <boden/layout/rect.hpp>
#include <boden/widget/view_controller.hpp>
#include <memory>
#include <miro/widget/hover_button.hpp>

namespace miro {

class zoom_delegate_t {
public:
    virtual void did_zoom_change(boden::widget::view_controller_ref_t sender,
                                 uint32_t zoom) = 0;
    
    virtual ~zoom_delegate_t() = default;
};

class zoom_view_controller_t : public boden::widget::view_controller_t
{
public:
    zoom_view_controller_t();
    zoom_view_controller_t(const boden::layout::rect_t &frame);
    ~zoom_view_controller_t();
    
    void load_view() override;
    void view_did_load() override;
    
    void on_zoom_in_click(void *sender);
    void on_zoom_out_click(void *sender);
    
    void set_zoom_delegate(miro::zoom_delegate_t *delegate);
    
private:
    miro::zoom_delegate_t *_zoom_delegate;
    
    uint8_t _zoom_index;
    std::shared_ptr<miro::widget::hover_button_t> _zoom_in;
    std::shared_ptr<miro::widget::hover_button_t> _zoom_out;
    
    void init();
};

} // miro
