#ifndef WEBGL_RENDERER_HPP
#define WEBGL_RENDERER_HPP

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
EMSCRIPTEN_KEEPALIVE
#endif

#ifdef __cplusplus
extern "C" {
#endif

int increment(int value);

#ifdef __cplusplus
}
#endif

#endif