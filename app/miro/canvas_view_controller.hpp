#pragma once

#include <boden/layout/rect.hpp>
#include <boden/widget/view.hpp>
#include <boden/widget/view_controller.hpp>
#include <memory>
#include <miro/canvas_view.hpp>
#include <unordered_set>

namespace miro {

class canvas_view_controller_t : public boden::widget::view_controller_t,
                                 public miro::canvas_view_delegate_t
{
public:
    canvas_view_controller_t();
    ~canvas_view_controller_t();
    
    void load_view() override;
    void did_canvas_view_add_subview(const boden::widget::view_t *view) override;
    void did_canvas_view_key_down(uint32_t key_code, uint64_t modifier_flags) override;
    void did_canvas_view_key_up(uint32_t key_code, uint64_t modifier_flags) override;
    void did_canvas_view_mouse_down(const boden::layout::point_t &location) override;
    void did_canvas_view_mouse_dragged(const boden::layout::point_t &location) override;
    void did_canvas_view_mouse_up(const boden::layout::point_t &location) override;
    void did_canvas_view_scroll_wheel(const boden::layout::vec2_t &delta) override;
    
    void set_zoom(uint32_t zoom);
    
private:
    void init();
    
    boden::layout::point_t _offset;
};

} // miro
