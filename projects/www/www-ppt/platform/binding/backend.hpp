#pragma once

#include "../www_backend.hpp"

#include <emscripten.h>
#include <emscripten/bind.h>

namespace platform {
namespace binding {

EMSCRIPTEN_BINDINGS(backend) 
{
    emscripten::class_<boden::backend_t>("backend_t");

    emscripten::class_<platform::www_backend_t, emscripten::base<boden::backend_t>>("www_backend_t")
        .constructor<>()
        .function("commitTextInput", &platform::www_backend_t::commit_text_input)
        .function("mouseDown", &platform::www_backend_t::mouse_down)
        .function("mouseDragged", &platform::www_backend_t::mouse_dragged)
        .function("mouseMoved", &platform::www_backend_t::mouse_moved)
        .function("mouseUp", &platform::www_backend_t::mouse_up)
        .function("draw", &platform::www_backend_t::draw);
}

} // binding
} // platform
