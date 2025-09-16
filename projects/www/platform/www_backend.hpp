#pragma once

#include <app/main_window.hpp>
#include <boden/backend.hpp>
#include <boden/builder.hpp>
#include <boden/event.hpp>
#include <memory>

namespace platform {

class www_backend_t : public boden::backend_t
{
public:
    www_backend_t();
    ~www_backend_t();

    void draw(boden::builder_t &builder);
    void try_run(const boden::event_t &event);

private:
    std::shared_ptr<app::main_window_t> _window;
};

} // platform
