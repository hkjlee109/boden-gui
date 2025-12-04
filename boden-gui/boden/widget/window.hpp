#pragma once

#include <boden/backend.hpp>
#include <boden/builder.hpp>
#include <boden/layout/rect.hpp>
#include <boden/graphic/texture_id.hpp>
#include <boden/graphic/texture_manager.hpp>
#include <boden/system_event.hpp>
#include <boden/tracking_area_manager.hpp>
#include <boden/widget/base/responder.hpp>
#include <memory>

namespace boden {
namespace widget {

class view_t;
class view_controller_t;

using view_ref_t = std::shared_ptr<view_t>; 
using view_controller_ref_t = std::shared_ptr<view_controller_t>; 

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
    void set_content_view(boden::widget::view_ref_t view);
    void set_content_view_controller(boden::widget::view_controller_ref_t ctrl);
    
    const boden::layout::rect_t & get_frame() const;
    void set_frame(const boden::layout::rect_t &frame);
    
    void set_needs_display(bool needs);
    void set_texture_manager(boden::graphic::texture_manager_t *texture_manager);
    float get_backing_scale_factor() const; 

    boden::graphic::texture_id_t create_view_texture(const boden::layout::size_t &size);
    void destroy_view_texture(boden::graphic::texture_id_t tid);

    void system(const boden::system_event_t &system_event);
    void enqueue_system_event(const boden::system_event_t &system_event);

    bool make_first_responder(std::shared_ptr<boden::widget::base::responder_t> responder);

protected:
    boden::layout::rect_t _frame;
    boden::widget::view_ref_t _content_view;
    boden::widget::view_controller_ref_t _content_view_controller;
    std::shared_ptr<boden::widget::base::responder_t> _first_responder;

    boden::backend_t *_backend;
    boden::graphic::texture_manager_t *_texture_manager;
    boden::tracking_area_manager_t _tracking_area_manager;

    float _backing_scale_factor;
};

} // widget
} // boden
