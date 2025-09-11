#pragma once

#include <boden/event.hpp>

#include <emscripten.h>
#include <emscripten/bind.h>

namespace platform {
namespace binding {

EMSCRIPTEN_BINDINGS(event) 
{
    emscripten::enum_<boden::event_type_t>("event_type_t")
        .value("draw", boden::event_type_t::draw)
        .value("left_mouse_down", boden::event_type_t::left_mouse_down)
        .value("left_mouse_dragged", boden::event_type_t::left_mouse_dragged)
        .value("left_mouse_up", boden::event_type_t::left_mouse_up);

    emscripten::value_object<boden::event_t>("event_t")
        .field("type", &boden::event_t::type)
        .field("location", &boden::event_t::location);
}

} // binding
} // platform
