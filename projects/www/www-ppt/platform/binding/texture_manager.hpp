#pragma once

#include "../webgl_texture_manager.hpp"

#include <boden/gpu/texture_manager.hpp>

#include <emscripten.h>
#include <emscripten/bind.h>

namespace platform {
namespace binding {

EMSCRIPTEN_BINDINGS(texture_manager) 
{
    emscripten::class_<boden::gpu::texture_manager_t>("texture_manager_t");

    emscripten::class_<platform::webgl_texture_manager_t, emscripten::base<boden::gpu::texture_manager_t>>("webgl_texture_manager_t")
        .constructor<>();
}

} // binding
} // platform
