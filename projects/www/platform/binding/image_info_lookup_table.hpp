#pragma once

#include <boden/asset/image_info_lookup_table.hpp>

#include <emscripten.h>
#include <emscripten/bind.h>

namespace platform {
namespace binding {

EMSCRIPTEN_BINDINGS(image_info_lookup_table) 
{
    emscripten::class_<boden::asset::image_info_lookup_table_t>("image_info_lookup_table_t")
        .constructor<>()
        .function("insert", &boden::asset::image_info_lookup_table_t::insert);

    emscripten::class_<boden::asset::image_info_lookup_table_ref_t>("image_info_lookup_table_ref_t")
        .constructor<boden::asset::image_info_lookup_table_t *>();
}

} // binding
} // platform
