#pragma once

#include <boden/layout/point.hpp>

#include <emscripten.h>
#include <emscripten/bind.h>

namespace platform {
namespace binding {

EMSCRIPTEN_BINDINGS(point) 
{
    emscripten::value_object<boden::layout::point_t>("point_t")
        .field("x", &boden::layout::point_t::x)
        .field("y", &boden::layout::point_t::y);
}

} // binding
} // platform
