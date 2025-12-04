#pragma once

#include <boden/layout/point.hpp>
#include <boden/layout/rect.hpp>
#include <boden/widget/scroll_view.hpp>
#include <boden/widget/view.hpp>
#include <miro/widget/layer/grid_layer.hpp>
#include <memory>

namespace miro {

class canvas_view_delegate_t 
{
public:
    virtual void did_canvas_view_add_subview(const boden::widget::view_t *view) = 0;
    virtual void did_canvas_view_key_down(uint32_t key_code, uint64_t modifier_flags) = 0;
    virtual void did_canvas_view_key_up(uint32_t key_code, uint64_t modifier_flags) = 0;
    virtual void did_canvas_view_mouse_down(const boden::layout::point_t &location) = 0;
    virtual void did_canvas_view_mouse_dragged(const boden::layout::point_t &location) = 0;
    virtual void did_canvas_view_mouse_up(const boden::layout::point_t &location) = 0;
    virtual void did_canvas_view_scroll_wheel(const boden::layout::vec2_t &delta) = 0;
    virtual ~canvas_view_delegate_t() = default;
};

class canvas_view_t : public boden::widget::scroll_view_t
{
public:
    static std::shared_ptr<canvas_view_t> alloc(const boden::layout::rect_t &frame);

    canvas_view_t();
    canvas_view_t(const boden::layout::rect_t &frame);
    ~canvas_view_t() override;
    
    void draw_rect(boden::builder_t &builder, const boden::layout::rect_t &dirty_rect) override;
    void make_backing_layer() override;
    void view_will_move_to_window(std::shared_ptr<boden::widget::window_t> window) override;
    void set_frame(const boden::layout::rect_t &frame) override;

    bool accepts_first_responder() override;
    void did_add_subview(const boden::widget::view_t *view) override;
    boden::widget::view_ref_t hit_test(boden::layout::point_t point) override;
    void key_down(const boden::event_t &event) override;
    void key_up(const boden::event_t &event) override;
    void mouse_down(const boden::event_t &event) override;
    void mouse_dragged(const boden::event_t &event) override;
    void mouse_up(const boden::event_t &event) override;
    void scroll_wheel(const boden::event_t &event) override;
    
    void set_delegate(miro::canvas_view_delegate_t *delegate);
    void set_offset(const boden::layout::point_t &offset);
    void set_zoom(uint32_t zoom);

protected:
    void init(const boden::layout::rect_t &frame) override;

private:
    miro::canvas_view_delegate_t *_delegate;
    std::shared_ptr<miro::widget::layer::grid_layer_t> _grid_layer;
};

} // miro
