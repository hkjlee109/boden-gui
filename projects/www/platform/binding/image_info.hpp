#pragma once

#include <boden/asset/image_info.hpp>

#include <emscripten.h>
#include <emscripten/bind.h>

namespace platform {
namespace binding {

EMSCRIPTEN_BINDINGS(image_info) 
{
    emscripten::value_object<boden::asset::image_info_t>("image_info_t")
        .field("id", &boden::asset::image_info_t::id)
        .field("width", &boden::asset::image_info_t::width)
        .field("height", &boden::asset::image_info_t::height);
}

} // binding
} // platform
