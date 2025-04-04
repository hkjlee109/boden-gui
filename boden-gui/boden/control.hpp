#pragma once

#include <boden/layout/rect.hpp>
#include <boden/view.hpp>
#include <functional>
#include <unordered_map>
#include <vector>

namespace boden {

enum class control_event_t 
{
    touch_down,
    touch_up_inside,
};

class control_t : public view_t
{
public:
    control_t();
    control_t(const boden::layout::rect_t &frame);
    virtual ~control_t();

    template <typename T>
    void add_target(T* target, void (T::*method)(), control_event_t event)
    {
        _actions[event].push_back(std::bind(method, target));
    }

    void send_actions(control_event_t event);

    bool is_enabled() const;
    void set_enabled(bool enabled);

private:
    std::unordered_map<control_event_t, std::vector<std::function<void()>>> _actions;
    bool _enabled;
};

} // boden
