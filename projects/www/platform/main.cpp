#include <boden/renderer.hpp>
#include <boden/context.hpp>
#include <boden/layout/point.hpp>
#include <boden/layout/rect.hpp>
#include <boden/layout/size.hpp>
#include <app/main_view_controller.hpp>

#include <emscripten.h>
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(main)
{
    emscripten::class_<boden::context_t>("context_t")
        .constructor<>()
        .property("renderer", &boden::context_t::renderer, emscripten::allow_raw_pointers());
        
    emscripten::class_<boden::layout::rect_t>("rect_t")
        .constructor<float, float, float, float>()
        .property("origin", &boden::layout::rect_t::origin)
        .property("size", &boden::layout::rect_t::size);

    emscripten::class_<app::main_view_controller_t>("main_view_controller_t")
        .constructor<boden::layout::rect_t>();
}
