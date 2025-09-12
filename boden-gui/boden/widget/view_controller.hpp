#pragma once

#include <boden/layout/rect.hpp>
#include <boden/widget/base/responder.hpp>
#include <boden/widget/view.hpp>
#include <memory>

namespace boden {
namespace widget {

class view_controller_t : public boden::widget::base::responder_t,
                          public std::enable_shared_from_this<boden::widget::view_controller_t>
{
public:
    view_controller_t();
    view_controller_t(const boden::layout::rect_t &frame);
    ~view_controller_t() override;

    void mouse_down(const boden::event_t &ev) override;
    void mouse_dragged(const boden::event_t &ev) override;
    void mouse_up(const boden::event_t &ev) override;

    std::shared_ptr<boden::widget::view_t> get_view() const;

    void set_parent(const std::shared_ptr<const boden::widget::view_controller_t> &ctrl);

    void add_child_view_controller(const std::shared_ptr<boden::widget::view_controller_t> &ctrl);

protected:
    std::shared_ptr<boden::widget::view_t> _view;

    std::vector<std::shared_ptr<boden::widget::view_controller_t>> _child_view_controllers;
    std::weak_ptr<const boden::widget::view_controller_t> _parent;
};

} // widget
} // boden
