#pragma once

#include <boden/builder.hpp>
#include <boden/widget/base/layer.hpp>
#include <boden/widget/base/responder.hpp>
#include <boden/widget/window.hpp>
#include <boden/layout/rect.hpp>
#include <vector>

namespace boden::widget::base {
class tracking_area_t;
} // boden::widget::base 

namespace boden {
namespace widget {

class view_delegate_t {
public:
    virtual void did_view_mouse_down(std::shared_ptr<boden::widget::view_t> sender, 
                                     const boden::layout::point_t &location) = 0;

    virtual void did_view_mouse_dragged(std::shared_ptr<boden::widget::view_t> sender, 
                                        const boden::layout::point_t &location) = 0;

    virtual void did_view_mouse_up(std::shared_ptr<boden::widget::view_t> sender, 
                                   const boden::layout::point_t &location) = 0;

    virtual ~view_delegate_t() = default;
};

class view_t : public boden::widget::base::responder_t, 
               public std::enable_shared_from_this<boden::widget::view_t>
{
public:
    view_t();
    view_t(const boden::layout::rect_t &frame);
    ~view_t() override;

    boden::widget::base::layer_t layer;


    void mouse_down(const boden::event_t &ev) override;
    void mouse_dragged(const boden::event_t &ev) override;
    void mouse_up(const boden::event_t &ev) override;

    virtual void draw(boden::builder_t &builder);
    virtual std::shared_ptr<boden::widget::view_t> hit_test(boden::layout::point_t point);

    void set_view_delegate(boden::widget::view_delegate_t *delegate);

    const boden::layout::rect_t & get_frame() const;
    void set_frame(const boden::layout::rect_t& frame);

    const std::vector<std::shared_ptr<boden::widget::view_t>> & get_subviews() const;

    std::shared_ptr<const boden::widget::view_t> get_superview() const;
    void set_superview(const std::shared_ptr<const boden::widget::view_t> &view);

    void set_window(const std::shared_ptr<boden::widget::window_t> &window);

    bool is_hidden() const;
    void set_hidden(bool hidden);

    void set_needs_display(bool needs);

    const std::vector<std::shared_ptr<boden::widget::base::tracking_area_t>> & get_tracking_areas() const;

    void add_subview(const std::shared_ptr<boden::widget::view_t> &view);
    void add_tracking_area(const std::shared_ptr<boden::widget::base::tracking_area_t> &area);
    void remove_tracking_area(const std::shared_ptr<boden::widget::base::tracking_area_t> &area);

    boden::layout::point_t convert_point_to_view(const boden::layout::point_t &point, 
                                                 const boden::widget::view_t *to_view) const;

protected:
    boden::widget::view_delegate_t *_view_delegate;
    
    boden::layout::rect_t _bounds;
    boden::layout::rect_t _frame;
    bool _hidden;

    std::vector<std::shared_ptr<boden::widget::view_t>> _subviews;
    std::weak_ptr<const boden::widget::view_t> _superview;
    std::weak_ptr<boden::widget::window_t> _window;

    std::vector<std::shared_ptr<boden::widget::base::tracking_area_t>> _tracking_areas;
};

} // widget
} // boden
