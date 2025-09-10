#pragma once

#include <boden/builder.hpp>

#include <emscripten.h>
#include <emscripten/bind.h>

namespace platform {
namespace bridge {

EMSCRIPTEN_BINDINGS(builder) 
{
    emscripten::class_<boden::builder_t>("builder_t")
        .constructor<>()
        .function("get_batch", &boden::builder_t::get_batch);
}

} // bridge
} // platform
