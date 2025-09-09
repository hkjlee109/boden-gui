#pragma once

#include "webgl_image_library.hpp"
#include "webgl_renderer.hpp"

#include <boden/asset/image_library_ref.hpp>
#include <boden/context.hpp>
#include <boden/draw/command.hpp>
#include <boden/layout/point.hpp>
#include <boden/layout/rect.hpp>
#include <boden/layout/size.hpp>
#include <boden/renderer.hpp>
#include <app/main_view_controller.hpp>

#include <emscripten.h>
#include <emscripten/bind.h>

namespace platform {

std::shared_ptr<app::main_view_controller_t> make_main_view_controller(const boden::layout::rect_t& frame) 
{
    return std::make_shared<app::main_view_controller_t>(frame);
}

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

    emscripten::class_<boden::asset::image_library_t>("image_library_t");
    
    emscripten::class_<boden::asset::image_library_ref_t>("image_library_ref_t")
        .constructor<boden::asset::image_library_t *>();

    emscripten::class_<app::main_view_controller_t>("main_view_controller_t")
        .smart_ptr<std::shared_ptr<app::main_view_controller_t>>("main_view_controller_t")
        .function("draw", &app::main_view_controller_t::draw)
        .function("load_view", &app::main_view_controller_t::load_view);
    
    emscripten::function("make_main_view_controller", &make_main_view_controller);

    emscripten::class_<webgl_renderer_t, emscripten::base<boden::renderer_t>>("webgl_renderer_t")
        .constructor<>()
        .function("begin_draw", &webgl_renderer_t::begin_draw);
        
    emscripten::class_<webgl_image_library_t, emscripten::base<boden::asset::image_library_t>>("webgl_image_library_t")
        .constructor<>()
        .function("load_image_from_path", &webgl_image_library_t::load_image_from_path);
}

} // platform
