#pragma once

#include "../webgl_image_library.hpp"

#include <emscripten.h>
#include <emscripten/bind.h>

namespace platform {
namespace binding {

EMSCRIPTEN_BINDINGS(image_library) 
{
    emscripten::class_<boden::asset::image_library_t>("image_library_t");
    
    emscripten::class_<webgl_image_library_t, emscripten::base<boden::asset::image_library_t>>("webgl_image_library_t")
        .constructor<>()
        .function("load_image_from_path", &webgl_image_library_t::load_image_from_path);
}

} // binding
} // platform
