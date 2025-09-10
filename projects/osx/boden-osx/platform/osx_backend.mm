#include "osx_backend.hpp"

#include "utils.hpp"
#include <boden/context.hpp>
#include <boden/event.hpp>
#include <Cocoa/Cocoa.h>

namespace platform {

osx_backend_t::osx_backend_t(MTL::Device *device, platform::osx_queue_t &queue, id<RenderViewProvider> provider)
    : _queue{queue},
      _provider{provider}
{
    platform::mtl_image_library_t *mtl_image_library{new platform::mtl_image_library_t(device)};
    
    NSImage *nsimage = [NSImage imageNamed:@"gearshape"];
    boden::widget::base::image_t image;
    platform::utils_t::convert_to_image((__bridge void *)nsimage, &image);
    mtl_image_library->load_image_from_data("gearshape", image);
    
    _image_library = std::make_unique<boden::asset::image_library_ref_t>(mtl_image_library);
    _renderer = std::make_unique<platform::mtl_renderer_t>(device, mtl_image_library);
    _main_view_controller = std::make_shared<app::main_view_controller_t>(boden::layout::rect_t{0, 0, 640, 480});
    _main_view_controller->load_view();
}

osx_backend_t::~osx_backend_t()
{
    if(_thread.joinable())
    {
        _thread.join();
    }
}

void osx_backend_t::start()
{
    _thread = std::thread(&osx_backend_t::main, this);
}

void osx_backend_t::draw()
{
    auto builder = std::make_shared<boden::builder_t>();
    _main_view_controller->draw(*builder.get());

    dispatch_async(dispatch_get_main_queue(), ^{
        boden::context_t ctx;

        ctx.surface_handle = (boden::surface_handle_t)(__bridge CA::MetalDrawable *)_provider.currentDrawable;
        ctx.display_size = boden::layout::size_t{(float)_provider.displaySize.width,
                                                 (float)_provider.displaySize.height};
        ctx.display_scale = boden::layout::vec2_t{(float)_provider.displayScale,
                                                  (float)_provider.displayScale};
        ctx.batch = &builder->get_batch();
        _renderer->render(ctx);
    });
}

int osx_backend_t::main()
{
    std::string msg;
    while(true)
    {
        _queue.wait();
        auto event = _queue.front();
        _queue.pop();
        
        switch(event.type)
        {
            case boden::event_type_t::draw:
                draw();
                break;
                
            case boden::event_type_t::left_mouse_down:
                _main_view_controller->mouse_down(event);
                dispatch_async(dispatch_get_main_queue(), ^{
                    [_provider setNeedsDisplay:YES];
                });
                break;
                
            case boden::event_type_t::left_mouse_dragged:
                _main_view_controller->mouse_dragged(event);
                dispatch_async(dispatch_get_main_queue(), ^{
                    [_provider setNeedsDisplay:YES];
                });
                break;
                
            case boden::event_type_t::left_mouse_up:
                _main_view_controller->mouse_up(event);
                dispatch_async(dispatch_get_main_queue(), ^{
                    [_provider setNeedsDisplay:YES];
                });
                break;
        }
    }
}

} // platform
