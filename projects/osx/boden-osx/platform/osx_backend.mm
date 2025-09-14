#include "osx_backend.hpp"

#include "utils.hpp"
#include <boden/asset/stb_ref.hpp>
#include <boden/context.hpp>
#include <boden/event.hpp>
#include <Cocoa/Cocoa.h>

namespace platform {

osx_backend_t::osx_backend_t(MTL::Device *device, platform::osx_queue_t &queue, id<RenderViewProvider> provider)
    : _queue{queue},
      _provider{provider},
      _mtl_image_library{platform::mtl_image_library_t(device)}
{
    boden::asset::image_id_lookup_table_t *image_id_lookup_table{new boden::asset::image_id_lookup_table_t()};
    _mtl_image_library.set_image_id_lookup_table(image_id_lookup_table);
    
    NSString *path;
    path = [[NSBundle mainBundle] pathForResource:@"gearshape" ofType:@"png"];
    _mtl_image_library.load_image_from_path("gearshape", path.UTF8String);
    
    path = [[NSBundle mainBundle] pathForResource:@"rectangle" ofType:@"png"];
    _mtl_image_library.load_image_from_path("rectangle", path.UTF8String);
    
    _image_id_lookup_table = std::make_unique<boden::asset::image_id_lookup_table_ref_t>(image_id_lookup_table);
    
    _renderer = std::make_unique<platform::mtl_renderer_t>(device, &_mtl_image_library);
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
