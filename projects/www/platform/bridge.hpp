#pragma once

#include "webgl_renderer.hpp"

#include <boden/context.hpp>
#include <boden/layout/point.hpp>
#include <boden/layout/rect.hpp>
#include <boden/layout/size.hpp>
#include <boden/renderer.hpp>
#include <app/main_view_controller.hpp>

#include <emscripten.h>
#include <emscripten/bind.h>

namespace platform {

EMSCRIPTEN_BINDINGS(bridge) 
{
    emscripten::class_<boden::layout::rect_t>("rect_t")
        .constructor<float, float, float, float>()
        .property("origin", &boden::layout::rect_t::origin)
        .property("size", &boden::layout::rect_t::size);

    emscripten::class_<boden::renderer_t>("renderer_t");

    emscripten::class_<boden::context_t>("context_t")
        .constructor<>()
        .property("renderer", &boden::context_t::renderer, emscripten::allow_raw_pointers());

    emscripten::class_<app::main_view_controller_t>("main_view_controller_t")
        .constructor<boden::layout::rect_t>()
        .function("draw", &app::main_view_controller_t::draw);

    emscripten::class_<webgl_renderer_t, emscripten::base<boden::renderer_t>>("webgl_renderer_t")
        .constructor<>()
        .function("begin_draw", &webgl_renderer_t::begin_draw);
}

} // platform
