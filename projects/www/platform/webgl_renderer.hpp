#pragma once

#include <boden/renderer.hpp>

#include <emscripten.h>
#include <emscripten/bind.h>

namespace platform {

class webgl_renderer_t : public boden::renderer_t
{
public:
    webgl_renderer_t();
    ~webgl_renderer_t();

    void begin_draw(boden::context_t &ctx) override;
    void end_draw(boden::context_t &ctx) override;
};

EMSCRIPTEN_BINDINGS(webgl_renderer) 
{
    emscripten::class_<boden::renderer_t>("renderer_t");
// emscripten::class_<dog_t, emscripten::base<animal_t>>("dog_t")
//     .constructor<std::string, int>()
//     .function("speak", &dog_t::speak)
//     .property("name", &dog_t::get_name, &dog_t::set_name);

    // emscripten::value_object<command_t>("command_t")
    //     .field("count", &command_t::count)
    //     .field("index_buffer_offset", &command_t::index_buffer_offset)
    //     .field("vertex_buffer_offset", &command_t::vertex_buffer_offset);

    // emscripten::class_<webgl_renderer_t, emscripten::base<boden::renderer_t>>("webgl_renderer_t")
    //     .constructor<>();
        // .function("begin_draw", &webgl_renderer_t::begin_draw);
}

} // platform
