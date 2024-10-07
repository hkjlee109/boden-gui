#pragma once

#include "Metal/Metal.hpp"
#include "QuartzCore/CAMetalDrawable.hpp"

namespace boden {

class renderer_t {
public:
    renderer_t(CA::MetalDrawable* drawable, MTL::Device* device);
    ~renderer_t();
    
    void draw_rect();
    
private:
    CA::MetalDrawable* _drawable;
    MTL::Device* _device;
    MTL::CommandQueue* _commandQueue;
};

} // boden
