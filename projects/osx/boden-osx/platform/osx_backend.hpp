#pragma once

#include "mtl_image_library.hpp"
#include "mtl_renderer.hpp"
#include "osx_queue.hpp"

#include <app/main_view_controller.hpp>
#include <boden/asset/image_library_ref.hpp>
#include <boden/widget/base/image.hpp>

#include <thread>

namespace platform {

class osx_backend_t
{
public:
    osx_backend_t(platform::osx_queue_t &queue);
    ~osx_backend_t();
    
    void start();

private:
    std::thread _thread;
    platform::osx_queue_t &_queue;


    std::unique_ptr<app::main_view_controller_t> _main_view_controller;
    std::unique_ptr<boden::asset::image_library_ref_t> _image_library;
    std::unique_ptr<platform::mtl_renderer_t> _renderer;
    
    int main();
};

} // platform
