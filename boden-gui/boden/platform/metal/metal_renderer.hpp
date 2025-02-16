#pragma once

#include <boden/math/vec.hpp>
#include <boden/renderer.hpp>

#include "Metal/Metal.hpp"
#include "QuartzCore/CAMetalDrawable.hpp"

namespace boden {
namespace platform {
namespace metal {

class metal_renderer_t : public renderer_t
{
public:
    metal_renderer_t(CA::MetalDrawable* drawable, MTL::Device* device);
    ~metal_renderer_t();
    
    void draw_rect();
    
private:
    void setup_pipeline();
    CA::MetalDrawable *_drawable;
    MTL::Device *_device;
    MTL::CommandQueue *_commandQueue;
    MTL::RenderPipelineState *_pipeline;
};

} // metal
} // platform
} // boden
