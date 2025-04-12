#include <boden/context.hpp>
#include <app/main_view_controller.hpp>

#include <emscripten.h>
#include <emscripten/bind.h>


EMSCRIPTEN_BINDINGS(main)
{
    emscripten::value_object<boden::context_t>("context_t");

    emscripten::class_<app::main_view_controller_t>("main_view_controller")
        .constructor<>();
}
