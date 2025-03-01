#pragma once

#include <boden/math/vec.hpp>
#include <boden/renderer.hpp>

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalDrawable.hpp>

namespace platform {

using command_queue_ref_t = std::unique_ptr<MTL::CommandQueue, void(*)(MTL::CommandQueue *)>;
using pipeline_ref_t = std::unique_ptr<MTL::RenderPipelineState, void(*)(MTL::RenderPipelineState *)>;
using depth_stencil_state_ref_t = std::unique_ptr<MTL::DepthStencilState, void(*)(MTL::DepthStencilState *)>;
using texture_ref_t = std::unique_ptr<MTL::Texture, void(*)(MTL::Texture *)>;

class metal_renderer_t : public boden::renderer_t
{
public:
    metal_renderer_t(CA::MetalDrawable* drawable, MTL::Device* device);
    ~metal_renderer_t();
    
    void begin_paint() override;
    void end_paint() override;
    
    void draw_rect() override;
    
private:
    void setup_depth_stencil_state();
    void setup_render_pipeline();
    void setup_default_texture();

    CA::MetalDrawable *_drawable;
    MTL::Device *_device;
    
    command_queue_ref_t _command_queue;
    pipeline_ref_t _render_pipeline;
    depth_stencil_state_ref_t _depth_stencil_state;
    texture_ref_t _texture;
};

} // platform
