#pragma once

#include <boden/backend.hpp>
#include <boden/builder.hpp>
#include <boden/layout/rect.hpp>
#include <boden/system_event.hpp>
#include <boden/tracking_area_manager.hpp>
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

    virtual void order_front();
    virtual void draw(boden::builder_t &builder);


    void mouse_down(const boden::event_t &system_event) override;
    void mouse_dragged(const boden::event_t &system_event) override;
    void mouse_moved(const boden::event_t &system_event) override;
    void mouse_up(const boden::event_t &system_event) override;
    void key_down(const boden::event_t &system_event) override;
    void key_up(const boden::event_t &system_event) override;
    void scroll_wheel(const boden::event_t &system_event) override;

    void set_backend(boden::backend_t *backend);
    void set_content_view(std::shared_ptr<boden::widget::view_t> view);
    void set_content_view_controller(std::shared_ptr<boden::widget::view_controller_t> ctrl);
    void set_needs_display(bool needs);

    void system(const boden::system_event_t &system_event);
    void enqueue_system_event(const boden::system_event_t &system_event);

    bool make_first_responder(std::shared_ptr<boden::widget::base::responder_t> responder);

protected:
    std::shared_ptr<boden::widget::view_t> _content_view;
    std::shared_ptr<boden::widget::view_controller_t> _content_view_controller;
    std::shared_ptr<boden::widget::base::responder_t> _first_responder;

    boden::backend_t *_backend;
    boden::tracking_area_manager_t _tracking_area_manager;
};

} // widget
} // boden
