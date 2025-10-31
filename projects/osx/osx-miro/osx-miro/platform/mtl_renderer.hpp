#pragma once

#include <boden/context.hpp>
#include <boden/layout/vec.hpp>
#include <boden/renderer.hpp>

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalDrawable.hpp>

namespace platform {

using command_queue_ref_t = std::unique_ptr<MTL::CommandQueue, void(*)(MTL::CommandQueue *)>;
using pipeline_ref_t = std::unique_ptr<MTL::RenderPipelineState, void(*)(MTL::RenderPipelineState *)>;
using depth_stencil_ref_t = std::unique_ptr<MTL::DepthStencilState, void(*)(MTL::DepthStencilState *)>;
using texture_ref_t = std::unique_ptr<MTL::Texture, void(*)(MTL::Texture *)>;

class mtl_renderer_t : public boden::renderer_t
{
public:
    mtl_renderer_t(MTL::Device* device);
    ~mtl_renderer_t() override;
    
    void render(boden::context_t &ctx) override;
    
private:
    void setup_depth_stencil();
    void setup_pipeline();
    void setup_texture();

    MTL::Device *_device;
    
    command_queue_ref_t _command_queue;
    depth_stencil_ref_t _depth_stencil;
    
    pipeline_ref_t _render_pipeline;
    
    texture_ref_t _texture_default;
};

} // platform
