#pragma once

#include <boden/layout/rect.hpp>
#include <boden/widget/base/tracking_area.hpp>
#include <boden/widget/button.hpp>
#include <memory>

namespace miro {
namespace widget {

class action_button_t : public boden::widget::button_t
{
public:
    static std::shared_ptr<action_button_t> alloc(const boden::layout::rect_t &frame);

    action_button_t();
    action_button_t(const boden::layout::rect_t &frame);
    ~action_button_t();

    void mouse_entered(const boden::event_t &ev) override;
    void mouse_exited(const boden::event_t &ev) override;

    bool is_selected() const;
    void set_selected(bool selected);

    void set_tracking_enabled(bool enabled);

protected:
    void init(const boden::layout::rect_t &frame) override;

private:
    std::shared_ptr<boden::widget::base::tracking_area_t> _tracking_area;

    bool _mouse_entered;
    bool _selected;
    
    void update_background_color();
};

} // widget
} // miro
