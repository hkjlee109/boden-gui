#pragma once

#include "metal_image_library.hpp"

#include <boden/context.hpp>
#include <boden/layout/vec.hpp>
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
    metal_renderer_t(MTL::Device* device, metal_image_library_t *image_library);
    ~metal_renderer_t();
    
    void begin_draw(boden::context_t &ctx) override;
    void end_draw(boden::context_t &ctx) override;
    
private:
    void setup_depth_stencil_state();
    void setup_render_pipeline();
    void setup_default_texture();

    MTL::Device *_device;
    metal_image_library_t *_image_library;
    
    command_queue_ref_t _command_queue;
    pipeline_ref_t _render_pipeline;
    depth_stencil_state_ref_t _depth_stencil_state;
    texture_ref_t _texture;
};

} // platform
