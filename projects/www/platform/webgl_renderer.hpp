#pragma once

#include <emscripten.h>
#include <emscripten/bind.h>
EMSCRIPTEN_KEEPALIVE

extern "C" 
{

class webgl_renderer_t
{
public:
    webgl_renderer_t();
    ~webgl_renderer_t();
};
    
int increment(int value);

}
