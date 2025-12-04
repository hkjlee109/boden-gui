#include "osx_backend.hpp"

#include "utils.hpp"
#include <boden/asset/stb_ref.hpp>
#include <boden/context.hpp>
#include <boden/event.hpp>
#include <boden/utils/config.hpp>
#include <fstream>
#include <iostream>
#include <memory>
#include <miro/main_view_controller.hpp>
#include <sstream>

#include <Cocoa/Cocoa.h>

namespace platform {

osx_backend_t::osx_backend_t(MTL::Device *device, platform::osx_queue_t &queue, id<RenderViewProvider> provider)
    : boden::backend_t(),
      _queue{queue},
      _provider{provider},
      _mtl_texture_manager{platform::mtl_texture_manager_t(device)}
{
    _image_manager.set_texture_manager(&_mtl_texture_manager);
    _font_manager.set_texture_manager(&_mtl_texture_manager);

    NSString *images_config_path = [[NSBundle mainBundle] pathForResource:@"images-config" ofType:@"json"];
    auto images_configs = boden::utils::config_t::parse_images_config_file(images_config_path.UTF8String);
        
    for(auto config : images_configs)
    {
        NSString *path = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:config.name.c_str()]
                                                         ofType:[NSString stringWithUTF8String:config.type.c_str()]];
        
        _image_manager.load(config.key, path.UTF8String, config.scale);
    }

    NSString *fonts_config_path = [[NSBundle mainBundle] pathForResource:@"fonts-config" ofType:@"json"];
    auto fonts_configs = boden::utils::config_t::parse_fonts_config_file(fonts_config_path.UTF8String);
        
    for(auto config : fonts_configs)
    {
        NSString *path = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:config.name.c_str()]
                                                         ofType:[NSString stringWithUTF8String:config.type.c_str()]];
        _font_manager.load(path.UTF8String);
    }

    _builder = std::make_unique<boden::builder_t>();
    _builder->set_image_manager(&_image_manager);
    _builder->set_font_manager(&_font_manager);
    
    _renderer = std::make_unique<platform::mtl_renderer_t>(device);
    _renderer->set_texture_manager(&_mtl_texture_manager);
    
    _window = std::make_shared<miro::main_window_t>(boden::layout::rect_t{0, 0, 640, 480});
    _window->set_backend(this);
    _window->set_texture_manager(&_mtl_texture_manager);
    _window->set_content_view_controller(std::make_shared<miro::main_view_controller_t>());
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
    _builder->reset();
    _window->draw(*_builder.get());

    auto batch = _builder->get_batch();
        
    dispatch_async(dispatch_get_main_queue(), ^{
        auto strong_batch = batch;
        
        boden::context_t ctx;
        ctx.surface_handle = (boden::surface_handle_t)(__bridge CA::MetalDrawable *)_provider.currentDrawable;
        ctx.display_size = boden::layout::size_t{(float)_provider.displaySize.width,
                                                 (float)_provider.displaySize.height};
        ctx.display_scale = _window->get_backing_scale_factor();
        ctx.batch = strong_batch;
        _renderer->render(ctx);
    });
}

void osx_backend_t::display_if_needed()
{
    if(needs_display())
    {
        set_needs_display(false);
        dispatch_async(dispatch_get_main_queue(), ^{
            [_provider setNeedsDisplay:YES];
        });
    }
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
                display_if_needed();
                break;
                
            case boden::event_type_t::left_mouse_dragged:
                _window->mouse_dragged(event);
                display_if_needed();
                break;
                
            case boden::event_type_t::left_mouse_up:
                _window->mouse_up(event);
                display_if_needed();
                break;
                
            case boden::event_type_t::mouse_moved:
                _window->mouse_moved(event);
                display_if_needed();
                break;

            case boden::event_type_t::key_down:
                _window->key_down(event);
                display_if_needed();
                break;

            case boden::event_type_t::key_up:
                _window->key_up(event);
                display_if_needed();
                break;

            case boden::event_type_t::scroll_wheel:
                _window->scroll_wheel(event);
                display_if_needed();
                break;

            case boden::event_type_t::system:
                auto system_event = std::any_cast<const boden::system_event_t &>(event.params.at("system_event"));
                _window->system(system_event);
                display_if_needed();
                break;
        }

        while(needs_system())
        {
            auto event_ = dequeue_system_event();
            if(!event_.has_value())
            {
                continue;
            }
            
            boden::system_event_t event = *event_;
            switch(event.type)
            {
                case (uint32_t)boden::system_event_type_t::text_input_begin:
                {
                    auto text_ = std::any_cast<const std::string &>(event.params.at("text"));
                    auto frame_ = std::any_cast<const boden::layout::rect_t &>(event.params.at("frame"));
                    
                    NSString *text = [NSString stringWithUTF8String:text_.c_str()];
                    NSRect frame = NSMakeRect(frame_.origin.x,
                                              frame_.origin.y,
                                              frame_.size.width,
                                              frame_.size.height);
                    
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [_provider beginTextInput:text frame:frame];
                    });
                    break;
                }
                    
                case (uint32_t)boden::system_event_type_t::text_input_end:
                {
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [_provider endTextInput];
                    });
                    break;
                }
                    
                default:
                    break;
            }
        }
    }
}

} // platform
