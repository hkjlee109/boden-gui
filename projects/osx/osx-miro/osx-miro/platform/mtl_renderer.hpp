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
    void setup_white_texture();
    
    void create_default_pipeline(MTL::Library *library);
    void create_grid_pipeline(MTL::Library *library);
    void create_premultiplied_pipeline(MTL::Library *library);
    
    void create_root_texture_if_needed(const boden::layout::size_t &size);
    
    MTL::Device *_device;
    
    command_queue_ref_t _command_queue;
    depth_stencil_ref_t _depth_stencil;
    
    pipeline_ref_t _grid_pipeline;
    pipeline_ref_t _default_pipeline;
    pipeline_ref_t _premultiplied_pipeline;
    
    texture_ref_t _root_texture;
    texture_ref_t _white_texture;
};

} // platform
