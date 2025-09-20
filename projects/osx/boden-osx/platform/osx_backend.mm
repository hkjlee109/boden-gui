#include "osx_backend.hpp"

#include "utils.hpp"
#include <boden/asset/stb_ref.hpp>
#include <boden/context.hpp>
#include <boden/event.hpp>
#include <boden/utils/config.hpp>
#include <Cocoa/Cocoa.h>


#include <fstream>
#include <sstream>
#include <iostream>


namespace platform {

osx_backend_t::osx_backend_t(MTL::Device *device, platform::osx_queue_t &queue, id<RenderViewProvider> provider)
    : _queue{queue},
      _provider{provider},
      _mtl_image_library{platform::mtl_image_library_t(device)}
{
    auto *image_info_lookup_table{new boden::asset::image_info_lookup_table_t()};
    _mtl_image_library.set_image_info_lookup_table(image_info_lookup_table);
    
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"images-config" ofType:@"json"];
    auto configs = boden::utils::config_t::parse_images_config_file(jsonPath.UTF8String);
        
    for(auto config : configs)
    {
        NSString *path = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:config.name.c_str()]
                                                         ofType:[NSString stringWithUTF8String:config.type.c_str()]];
        _mtl_image_library.load_image_from_path(config.key, path.UTF8String);
    }

    _image_info_lookup_table = std::make_unique<boden::asset::image_info_lookup_table_ref_t>(image_info_lookup_table);
    
    _renderer = std::make_unique<platform::mtl_renderer_t>(device, &_mtl_image_library);
    _window = std::make_shared<app::main_window_t>(boden::layout::rect_t{0, 0, 640, 480});
    _window->set_backend(this);
    _window->order_front();
}

osx_backend_t::~osx_backend_t()
{
    if(_thread.joinable())
    {
        _thread.join();
    }
}

void osx_backend_t::run()
{
    _thread = std::thread(&osx_backend_t::main, this);
}

void osx_backend_t::draw()
{
    auto builder = std::make_shared<boden::builder_t>();
    _window->draw(*builder.get());

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
                _window->mouse_down(event);
                if(needs_display())
                {
                    set_needs_display(false);
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [_provider setNeedsDisplay:YES];
                    });
                }
                break;
                
            case boden::event_type_t::left_mouse_dragged:
                _window->mouse_dragged(event);
                if(needs_display())
                {
                    set_needs_display(false);
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [_provider setNeedsDisplay:YES];
                    });
                }
                break;
                
            case boden::event_type_t::left_mouse_up:
                _window->mouse_up(event);
                if(needs_display())
                {
                    set_needs_display(false);
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [_provider setNeedsDisplay:YES];
                    });
                }
                break;
                
            case boden::event_type_t::mouse_moved:
                _window->mouse_moved(event);
                if(needs_display())
                {
                    set_needs_display(false);
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [_provider setNeedsDisplay:YES];
                    });
                }
                break;
        }
    }
}

} // platform
