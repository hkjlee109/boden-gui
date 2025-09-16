#pragma once

#include <boden/backend.hpp>
#include <boden/layout/rect.hpp>
#include <boden/widget/base/responder.hpp>
#include <memory>

namespace boden {
namespace widget {

class view_t;
class view_controller_t;

class window_t : public boden::widget::base::responder_t,
                 public std::enable_shared_from_this<boden::widget::window_t>
{
public:
    window_t();
    window_t(const boden::layout::rect_t &frame);
    ~window_t() override;

    void mouse_down(const boden::event_t &ev) override;
    void mouse_dragged(const boden::event_t &ev) override;
    void mouse_moved(const boden::event_t &ev) override;
    void mouse_up(const boden::event_t &ev) override;

    void set_backend(boden::backend_t *backend);
    void set_needs_display(bool needs);

    void set_content_view(const std::shared_ptr<boden::widget::view_t> &view);
    void set_content_view_controller(const std::shared_ptr<boden::widget::view_controller_t> &ctrl);

    virtual void order_front();
    
protected:
    std::shared_ptr<boden::widget::view_t> _content_view;
    std::shared_ptr<boden::widget::view_controller_t> _content_view_controller;

    boden::backend_t *_backend;
};

} // widget
} // boden
