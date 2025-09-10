#pragma once

#include <app/main_view_controller.hpp>
#include <boden/layout/rect.hpp>
#include <memory>

#include <emscripten.h>
#include <emscripten/bind.h>

namespace platform {
namespace bridge {

std::shared_ptr<app::main_view_controller_t> make_main_view_controller(const boden::layout::rect_t &frame) 
{
    return std::make_shared<app::main_view_controller_t>(frame);
}

EMSCRIPTEN_BINDINGS(main_view_controller) 
{
    emscripten::class_<app::main_view_controller_t>("main_view_controller_t")
        .smart_ptr<std::shared_ptr<app::main_view_controller_t>>("main_view_controller_t")
        .function("draw", &app::main_view_controller_t::draw)
        .function("load_view", &app::main_view_controller_t::load_view);
    
    emscripten::function("make_main_view_controller", &make_main_view_controller);
}

} // bridge
} // platform
