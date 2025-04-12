#include <boden/context.hpp>

#include <emscripten.h>
#include <emscripten/bind.h>


EMSCRIPTEN_BINDINGS(main)
{
    emscripten::value_object<boden::context_t>("context_t");

}
