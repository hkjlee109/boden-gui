#include "www_backend.hpp"

#include <boden/context.hpp>
#include <boden/utils/config.hpp>
#include <fstream>

namespace platform {
    
www_backend_t::www_backend_t()
    : boden::backend_t(),
      _webgl_texture_manager{}
{
    _image_manager.set_texture_manager(&_webgl_texture_manager);
    _font_manager.set_texture_manager(&_webgl_texture_manager);

    auto images_configs = boden::utils::config_t::parse_images_config_file("./src/gui-backend/assets/images/images-config.json");
    for(auto config : images_configs)
    {
        _image_manager.load(config.key, "./src/gui-backend/assets/images/" + config.name + "." + config.type);
    }

    auto fonts_configs = boden::utils::config_t::parse_fonts_config_file("./src/gui-backend/assets/fonts/fonts-config.json");
    for(auto config : fonts_configs)
    {
        _font_manager.load("./src/gui-backend/assets/fonts/" + config.name + "." + config.type);
    }

    _builder = std::make_unique<boden::builder_t>();
    _builder->set_image_manager(&_image_manager);
    _builder->set_font_manager(&_font_manager);
    
    _renderer = std::make_unique<platform::webgl_renderer_t>();
    _renderer->set_texture_manager(&_webgl_texture_manager);

    _render_view_provider = std::make_unique<platform::render_view_provider_t>();

    _window = std::make_shared<miro::main_window_t>(boden::layout::rect_t{0, 0, 640, 480});
    _window->set_backend(this);
    _window->set_texture_manager(&_webgl_texture_manager);
    _window->order_front();
}

www_backend_t::~www_backend_t()
{
}

void www_backend_t::commit_text_input(const std::string &text)
{
    boden::system_event_t system_event;
    system_event.type = (uint32_t)boden::system_event_type_t::text_input_commit;
    system_event.params["text"] = text;

    _window->system(system_event);

    display_if_needed();
    process_system_event_if_needed();
}

void www_backend_t::key_down(uint32_t key_code, uint32_t flags_msb, uint32_t flags_lsb)
{
    boden::event_t event;
    event.type = boden::event_type_t::key_down;
    event.key_code = key_code;
    event.modifier_flags_msb = flags_msb;
    event.modifier_flags_lsb = flags_lsb;

    _window->key_down(event);
    
    display_if_needed();
    process_system_event_if_needed();
}

void www_backend_t::mouse_down(float x, float y)
{
    boden::event_t event;
    event.type = boden::event_type_t::left_mouse_down;
    event.location = {x, y};    
    
    _window->mouse_down(event);

    display_if_needed();
    process_system_event_if_needed();
}

void www_backend_t::mouse_dragged(float x, float y)
{
    boden::event_t event;
    event.type = boden::event_type_t::left_mouse_down;
    event.location = {x, y};    
    
    _window->mouse_dragged(event);

    display_if_needed();
    process_system_event_if_needed();
}

void www_backend_t::mouse_moved(float x, float y)
{
    boden::event_t event;
    event.type = boden::event_type_t::left_mouse_down;
    event.location = {x, y};    
    
    _window->mouse_moved(event);

    display_if_needed();
    process_system_event_if_needed();
}

void www_backend_t::mouse_up(float x, float y)
{
    boden::event_t event;
    event.type = boden::event_type_t::left_mouse_down;
    event.location = {x, y};    
    
    _window->mouse_up(event);

    display_if_needed();
    process_system_event_if_needed();
}

void www_backend_t::draw()
{
    _builder->reset();

    _window->draw(*_builder.get());

    boden::context_t ctx;
    ctx.batch = _builder->get_batch();
    _renderer->render(ctx);
}

void www_backend_t::display_if_needed()
{
    if(needs_display())
    {
        set_needs_display(false);
        draw();
    }
}

void www_backend_t::process_system_event_if_needed()
{
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
                auto text = std::any_cast<const std::string &>(event.params.at("text"));
                auto frame = std::any_cast<const boden::layout::rect_t &>(event.params.at("frame"));
                _render_view_provider->begin_text_input(text, frame);
                break;
            }
                    
            case (uint32_t)boden::system_event_type_t::text_input_end:
            {
                _render_view_provider->end_text_input();
                break;
            }
                    
            default:
                break;
        }
    }    
}

} // platform
