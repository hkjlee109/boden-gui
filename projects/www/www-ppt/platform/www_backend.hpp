#pragma once

#include "render_view_provider.hpp"
#include "webgl_renderer.hpp"
#include "webgl_texture_manager.hpp"

#include <boden/backend.hpp>
#include <boden/builder.hpp>
#include <boden/event.hpp>
#include <memory>
#include <ppt/main_window.hpp>

namespace platform {

class www_backend_t : public boden::backend_t
{
public:
    www_backend_t();
    ~www_backend_t();

    void commit_text_input(const std::string &text);
    void mouse_down(float x, float y);
    void mouse_dragged(float x, float y);
    void mouse_moved(float x, float y);
    void mouse_up(float x, float y);

    void draw();

private:
    std::unique_ptr<boden::builder_t> _builder;
    std::unique_ptr<platform::webgl_renderer_t> _renderer;
    std::unique_ptr<platform::render_view_provider_t> _render_view_provider;
    std::shared_ptr<ppt::main_window_t> _window;
    platform::webgl_texture_manager_t _webgl_texture_manager;

    void display_if_needed();
    void process_system_event_if_needed();
};

} // platform
