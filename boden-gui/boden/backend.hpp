#pragma once

#include <boden/asset/image_manager.hpp>
#include <boden/font/font_manager.hpp>
#include <boden/event.hpp>
#include <boden/system_event.hpp>
#include <memory>
#include <optional>
#include <queue>

namespace boden {

class backend_t
{
public:
    backend_t();
    virtual ~backend_t();

    bool needs_display() const;
    void set_needs_display(bool needs);

    bool needs_system() const;
    void enqueue_system_event(const boden::system_event_t &system_event);
    std::optional<boden::system_event_t> dequeue_system_event();

protected:
    boden::asset::image_manager_t _image_manager;
    boden::font::font_manager_t _font_manager;

private:
    class impl_t;
    std::unique_ptr<impl_t> _impl;

    std::queue<boden::system_event_t> _system_event_queue;
    bool _needs_display;
};

} // boden
