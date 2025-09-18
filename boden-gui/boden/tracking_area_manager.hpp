#pragma once

#include <boden/event.hpp>
#include <memory>

namespace boden::widget {
class view_t;
} // boden::widget

namespace boden::widget::base {
class tracking_area_t;
} // boden::widget::base

namespace boden {

class tracking_area_manager_t
{
public:
    tracking_area_manager_t();
    virtual ~tracking_area_manager_t();

    void set_content_view(const std::shared_ptr<boden::widget::view_t> &view);

    void mouse_moved(const boden::event_t &ev);

private:
    std::weak_ptr<boden::widget::view_t> _content_view;
    std::weak_ptr<boden::widget::base::tracking_area_t> _entered_area;
};

} // boden
