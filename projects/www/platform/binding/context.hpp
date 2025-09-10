#pragma once

#include <boden/context.hpp>

#include <emscripten.h>
#include <emscripten/bind.h>

namespace platform {
namespace bridge {

EMSCRIPTEN_BINDINGS(context) 
{
    emscripten::class_<boden::context_t>("context_t")
        .constructor<>()
        .property("batch", &boden::context_t::batch, emscripten::allow_raw_pointers());
}

} // bridge
} // platform
