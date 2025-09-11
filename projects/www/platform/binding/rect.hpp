#pragma once

#include <boden/layout/rect.hpp>

#include <emscripten.h>
#include <emscripten/bind.h>

namespace platform {
namespace binding {

EMSCRIPTEN_BINDINGS(rect) 
{
    emscripten::class_<boden::layout::rect_t>("rect_t")
        .constructor<float, float, float, float>()
        .property("origin", &boden::layout::rect_t::origin)
        .property("size", &boden::layout::rect_t::size);
}

} // binding
} // platform
