#pragma once

#include <boden/batch.hpp>

#include <emscripten.h>
#include <emscripten/bind.h>

namespace platform {
namespace bridge {

EMSCRIPTEN_BINDINGS(batch) 
{
    emscripten::class_<boden::batch_t>("batch_t")
        .constructor<>();
}

} // bridge
} // platform
