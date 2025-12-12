#pragma once

#include <boden/builder.hpp>
#include <boden/widget/layer/layer.hpp>
#include <boden/widget/base/responder.hpp>
#include <boden/widget/window.hpp>
#include <boden/layout/rect.hpp>
#include <boden/system_event.hpp>
#include <unordered_set>
#include <vector>

namespace boden::widget::base {
class tracking_area_t;
} // boden::widget::base 

namespace boden::widget::layout {
class layout_constraint_t;
class layout_x_axis_anchor_t;
using layout_constraint_ref_t = std::shared_ptr<boden::widget::layout::layout_constraint_t>;
using layout_x_axis_anchor_ref_t = std::shared_ptr<boden::widget::layout::layout_x_axis_anchor_t>;
} // boden::widget::layout 

namespace boden {
namespace widget {

using view_ref_t = std::shared_ptr<boden::widget::view_t>;
using view_wref_t = std::weak_ptr<boden::widget::view_t>;

class view_t : public boden::widget::base::responder_t, 
               public std::enable_shared_from_this<boden::widget::view_t>
{
public:
    static std::shared_ptr<view_t> alloc();
    static std::shared_ptr<view_t> alloc(const boden::layout::rect_t &frame);

    view_t();
    view_t(const boden::layout::rect_t &frame);
    ~view_t() override;

    view_t(const view_t &) noexcept = default;
    view_t& operator=(const view_t &) noexcept = default;

    view_t(view_t &&) noexcept = default;
    view_t& operator=(view_t &&) noexcept = default;

    virtual void draw_rect(boden::builder_t &builder, const boden::layout::rect_t &dirty_rect);
    virtual boden::widget::view_ref_t hit_test(boden::layout::point_t point);
    virtual void did_add_subview(const boden::widget::view_t *view);
    virtual void make_backing_layer();
    virtual void view_did_change_backing_properties();
    virtual void view_will_move_to_window(std::shared_ptr<boden::widget::window_t> window);

    virtual const boden::layout::rect_t & get_frame() const;
    virtual void set_frame(const boden::layout::rect_t &frame);

    const boden::layout::rect_t & get_bounds() const;

    std::shared_ptr<boden::widget::layer::layer_t> get_layer();

    const std::vector<boden::widget::view_ref_t> & get_subviews() const;

    boden::widget::view_ref_t get_superview() const;
    void set_superview(boden::widget::view_ref_t view);

    std::shared_ptr<const boden::widget::view_t> get_view_with_tag(uint32_t tag) const;

    std::shared_ptr<boden::widget::window_t> get_window() const;
    void set_window(std::shared_ptr<boden::widget::window_t> window);

    bool is_hidden() const;
    void set_hidden(bool hidden);

    void set_needs_display(bool needs);
    void set_needs_layout(bool needs);

    uint32_t get_tag() const;
    void set_tag(uint32_t tag);

    boden::widget::layout::layout_x_axis_anchor_ref_t get_leading_anchor() const;

    const std::vector<std::shared_ptr<boden::widget::base::tracking_area_t>> & get_tracking_areas() const;

    void add_subview(boden::widget::view_ref_t view);
    void remove_subview(boden::widget::view_ref_t view);
    void remove_from_superview();
    
    void add_constraint(boden::widget::layout::layout_constraint_ref_t constraint);
    void remove_constraint(boden::widget::layout::layout_constraint_ref_t constraint);

    void add_tracking_area(std::shared_ptr<boden::widget::base::tracking_area_t> area);
    void remove_tracking_area(std::shared_ptr<boden::widget::base::tracking_area_t> area);

    boden::layout::point_t convert_point_to_view(const boden::layout::point_t &point, 
                                                 const boden::widget::view_t *to_view) const;
    boden::layout::rect_t convert_rect_to_view(const boden::layout::rect_t &rect,
                                               const boden::widget::view_t *to_view) const;

    void layout();
    void layout_if_needed();
    void layout_subtree_if_needed();
    void layout_subviews();

    void update_constraints();
                                     
    void enqueue_system_event(const boden::system_event_t &event);

protected:
    boden::layout::rect_t _bounds;
    boden::layout::rect_t _frame;
    boden::layout::rect_t _dirty_rect;
    uint32_t _tag;
    bool _hidden;
    bool _needs_display;
    bool _needs_layout;

    std::vector<boden::widget::view_ref_t> _subviews;
    std::weak_ptr<boden::widget::view_t> _superview;
    std::weak_ptr<boden::widget::window_t> _window;

    std::shared_ptr<boden::widget::layer::layer_t> _layer;

    boden::widget::layout::layout_x_axis_anchor_ref_t _leading_anchor;
    
    std::unordered_set<boden::widget::layout::layout_constraint_ref_t> _constraints;

    std::vector<std::shared_ptr<boden::widget::base::tracking_area_t>> _tracking_areas;

    virtual void init();
    virtual void init(const boden::layout::rect_t &frame);

private:
    void create_layer_texture();
};

} // widget
} // boden
