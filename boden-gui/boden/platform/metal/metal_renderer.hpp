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
    
    void begin_paint() override;
    void end_paint() override;
    
    void draw_rect() override;
    
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
