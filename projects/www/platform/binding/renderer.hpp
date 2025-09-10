#pragma once

#include "../webgl_renderer.hpp"

#include <boden/renderer.hpp>

#include <emscripten.h>
#include <emscripten/bind.h>

namespace platform {
namespace bridge {

EMSCRIPTEN_BINDINGS(renderer) 
{
    emscripten::class_<boden::renderer_t>("renderer_t");

    emscripten::class_<webgl_renderer_t, emscripten::base<boden::renderer_t>>("webgl_renderer_t")
        .constructor<>()
        .function("render", &webgl_renderer_t::render);
}

} // bridge
} // platform
