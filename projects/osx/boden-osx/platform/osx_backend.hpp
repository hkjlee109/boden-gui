#pragma once

#include "mtl_image_library.hpp"
#include "mtl_renderer.hpp"
#include "osx_queue.hpp"

#include <app/main_view_controller.hpp>
#include <boden/asset/image_library.hpp>
#include <boden/asset/image_info_lookup_table.hpp>
#include <boden/backend.hpp>
#include <boden/widget/base/image.hpp>

#include <thread>
#include <Metal/Metal.hpp>

#include "RenderViewProvider.h"

namespace platform {

class osx_backend_t : public boden::backend_t
{
public:
    osx_backend_t(MTL::Device *device, platform::osx_queue_t &queue, id<RenderViewProvider> provider);
    ~osx_backend_t();
    
    void start();

private:
    std::thread _thread;
    platform::osx_queue_t &_queue;
    
    std::unique_ptr<boden::asset::image_info_lookup_table_ref_t> _image_info_lookup_table;
    std::unique_ptr<platform::mtl_renderer_t> _renderer;
    std::shared_ptr<app::main_view_controller_t> _main_view_controller;
    
    id<RenderViewProvider> _provider;
    platform::mtl_image_library_t _mtl_image_library;
    
    void draw();
    int main();
};

} // platform
