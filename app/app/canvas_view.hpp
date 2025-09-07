#pragma once

#include <boden/layout/rect.hpp>
#include <boden/widget/view.hpp>
#include <functional>

namespace app {

class canvas_view_t : public boden::widget::view_t
{
public:
    canvas_view_t();
    canvas_view_t(const boden::layout::rect_t &frame);
    ~canvas_view_t();

    void mouse_down(const boden::event_t &ev) override;
    void mouse_up(const boden::event_t &ev) override;

    void set_on_mouse_down(std::function<void(void *)> callback);
    void set_on_mouse_up(std::function<void(void *)> callback);
    
private:
    std::function<void(void *)> _on_mouse_down;
    std::function<void(void *)> _on_mouse_up;
};

} // app
