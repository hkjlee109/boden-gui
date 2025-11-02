#pragma once

#include <boden/layout/rect.hpp>
#include <boden/widget/base/tracking_area.hpp>
#include <boden/widget/button.hpp>
#include <memory>

namespace miro {
namespace widget {

class hover_button_t : public boden::widget::button_t
{
public:
    static std::shared_ptr<hover_button_t> alloc(const boden::layout::rect_t &frame);

    hover_button_t();
    hover_button_t(const boden::layout::rect_t &frame);
    ~hover_button_t();
        
    void mouse_entered(const boden::event_t &ev) override;
    void mouse_exited(const boden::event_t &ev) override;

    void set_hover_enabled(bool enabled);

protected:
    void init(const boden::layout::rect_t &frame) override;

private:
    std::shared_ptr<boden::widget::base::tracking_area_t> _tracking_area;
};

} // widget
} // miro
