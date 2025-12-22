#pragma once

#include "mtl_renderer.hpp"
#include "mtl_texture_manager.hpp"
#include "osx_queue.hpp"

#include <boden/backend.hpp>
#include <boden/widget/base/image.hpp>
#include <miro/main_window.hpp>

#include <thread>
#include <Metal/Metal.hpp>

#include "RenderViewProvider.h"

namespace platform {

class osx_backend_t : public boden::backend_t
{
public:
    osx_backend_t(MTL::Device *device, platform::osx_queue_t &queue, id<RenderViewProvider> provider);
    ~osx_backend_t();
    
    void run();

private:
    std::thread _thread;
    platform::osx_queue_t &_queue;

    std::unique_ptr<boden::builder_t> _builder;
    std::unique_ptr<platform::mtl_renderer_t> _renderer;
    std::shared_ptr<miro::main_window_t> _window;
    
    id<RenderViewProvider> _provider;
    platform::mtl_texture_manager_t _mtl_texture_manager;
    
    void draw();
    void display_if_needed();
    
    int main();
};

} // platform
