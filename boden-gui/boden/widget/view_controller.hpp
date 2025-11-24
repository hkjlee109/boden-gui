#pragma once

#include <boden/backend.hpp>
#include <boden/layout/rect.hpp>
#include <boden/widget/base/responder.hpp>
#include <boden/widget/view.hpp>
#include <memory>

namespace boden {
namespace widget {

using view_controller_ref_t = std::shared_ptr<boden::widget::view_controller_t>;

class view_controller_t : public boden::widget::base::responder_t,
                          public std::enable_shared_from_this<boden::widget::view_controller_t>
{
public:
    view_controller_t();
    ~view_controller_t() override;

    void mouse_down(const boden::event_t &ev) override;
    void mouse_dragged(const boden::event_t &ev) override;
    void mouse_up(const boden::event_t &ev) override;
 
    virtual void load_view();
    virtual void view_did_load();

    boden::widget::view_ref_t get_view();

    void set_parent(boden::widget::view_controller_ref_t ctrl);
    
    void add_child_view_controller(boden::widget::view_controller_ref_t ctrl);

protected:
    boden::widget::view_ref_t _view;

    boden::backend_t *_backend;
    std::vector<boden::widget::view_controller_ref_t> _child_view_controllers;
    std::weak_ptr<boden::widget::view_controller_t> _parent;
};

} // widget
} // boden
