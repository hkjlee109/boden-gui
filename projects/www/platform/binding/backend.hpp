#pragma once

#include "../www_backend.hpp"
#include <boden/layout/rect.hpp>

#include <emscripten.h>
#include <emscripten/bind.h>

namespace platform {
namespace binding {

EMSCRIPTEN_BINDINGS(backend) 
{
    emscripten::class_<boden::backend_t>("backend_t")
        .function("needs_display", &boden::backend_t::needs_display)
        .function("set_needs_display", &boden::backend_t::set_needs_display);

    emscripten::class_<platform::www_backend_t, emscripten::base<boden::backend_t>>("www_backend_t")
        .constructor<>()
        .function("draw", &platform::www_backend_t::draw)
        .function("try_run", &platform::www_backend_t::try_run);
}

} // binding
} // platform
