#pragma once

#include <boden/batch.hpp>

#include <emscripten.h>
#include <emscripten/bind.h>

namespace platform {
namespace binding {

EMSCRIPTEN_BINDINGS(batch) 
{
    emscripten::class_<boden::batch_t>("batch_t")
        .constructor<>()
        .function("get_commands_byte_size", &boden::batch_t::get_commands_byte_size)
        .function("get_indices_byte_size", &boden::batch_t::get_indices_byte_size)
        .function("get_vertices_byte_size", &boden::batch_t::get_vertices_byte_size)
        .function("serialize_commands", emscripten::optional_override([](const boden::batch_t &self, uintptr_t ptr, size_t size) {
            self.serialize_commands(reinterpret_cast<char*>(ptr), size);
        }))
        .function("serialize_indices", emscripten::optional_override([](const boden::batch_t &self, uintptr_t ptr, size_t size) {
            self.serialize_indices(reinterpret_cast<char*>(ptr), size);
        }))
        .function("serialize_vertices", emscripten::optional_override([](const boden::batch_t &self, uintptr_t ptr, size_t size) {
            self.serialize_vertices(reinterpret_cast<char*>(ptr), size);
        }))
        .function("deserialize_commands", emscripten::optional_override([](boden::batch_t &self, uintptr_t ptr, size_t size) {
            self.deserialize_commands(reinterpret_cast<const char*>(ptr), size);
        }))
        .function("deserialize_indices", emscripten::optional_override([](boden::batch_t &self, uintptr_t ptr, size_t size) {
            self.deserialize_indices(reinterpret_cast<const char*>(ptr), size);
        }))
        .function("deserialize_vertices", emscripten::optional_override([](boden::batch_t &self, uintptr_t ptr, size_t size) {
            self.deserialize_vertices(reinterpret_cast<const char*>(ptr), size);
        }));
}

} // binding
} // platform
