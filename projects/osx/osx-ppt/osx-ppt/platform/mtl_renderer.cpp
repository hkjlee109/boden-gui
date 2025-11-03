#include "mtl_renderer.hpp"

#include "mtl_buffer_manager.hpp"

#include <boden/draw/index.hpp>
#include <boden/draw/vertex.hpp>
#include <simd/simd.h>
#include <algorithm>
#include <iostream>

namespace platform {

static platform::mtl_buffer_manager_t buffer_manager;

struct main_uniforms_t
{
    float projection_matrix[4][4];
};

struct grid_uniforms_t
{
    float pan[2];
    float zoom;
    float grid_spacing;
    float screen_size[2];
    float padding[2];
};

mtl_renderer_t::mtl_renderer_t(MTL::Device *device)
    : boden::renderer_t(),
      _device{device},
      _command_queue{nullptr, [](MTL::CommandQueue *ptr) { if (ptr) ptr->release(); }},
      _depth_stencil{nullptr, [](MTL::DepthStencilState *ptr) { if (ptr) ptr->release(); }},
      _pipeline_default{nullptr, [](MTL::RenderPipelineState *ptr) { if (ptr) ptr->release(); }},
      _pipeline_premultiplied{nullptr, [](MTL::RenderPipelineState *ptr) { if (ptr) ptr->release(); }},
      _texture_default{nullptr, [](MTL::Texture *ptr) { if (ptr) ptr->release(); }}
{
    _command_queue.reset(_device->newCommandQueue());
    
    setup_pipeline();
    setup_depth_stencil();
    setup_texture();
}

mtl_renderer_t::~mtl_renderer_t()
{
}

void mtl_renderer_t::render(boden::context_t &ctx)
{
    boden::renderer_t::render(ctx);
    
    if(ctx.batch->command_groups.empty()
       || ctx.batch->indices.empty()
       || ctx.batch->vertices.empty())
    {
        return;
    }
    
    _texture_manager->cleanup_unused_texture();

    MTL::CommandBuffer *command_buffer = _command_queue->commandBuffer();
    
    mtl_buffer_ref_t vertex_buffer = buffer_manager.dequeue_reusable_buffer(_device,
                                                                            ctx.batch->vertices.size() * sizeof(boden::draw::vertex_t));
    mtl_buffer_ref_t index_buffer = buffer_manager.dequeue_reusable_buffer(_device,
                                                                           ctx.batch->indices.size() * sizeof(boden::draw::index_t));
    
    memcpy((char*)vertex_buffer->get_buffer()->contents(),
           ctx.batch->vertices.data(),
           ctx.batch->vertices.size() * sizeof(boden::draw::vertex_t));
    memcpy((char*)index_buffer->get_buffer()->contents(),
           ctx.batch->indices.data(),
           ctx.batch->indices.size() * sizeof(boden::draw::index_t));
    
    for(auto &command_group : ctx.batch->command_groups)
    {
        boden::layout::rect_t dst_frame = command_group.frame;
        boden::graphic::texture_id_t dst_tid = command_group.tid;
        boden::graphic::compositing_operation_t operation = command_group.operation;
        
        MTL::Texture *dst_texture = reinterpret_cast<MTL::Texture *>(
                _texture_manager->get_gpu_texture_handle(dst_tid));
        if(!dst_texture)
        {
            continue;
        }

        MTL::Viewport viewport =
        {
            .originX = 0.0,
            .originY = 0.0,
            .width = (double)(dst_frame.size.width),
            .height = (double)(dst_frame.size.height),
            .znear = 0.0,
            .zfar = 1.0
        };
        
        float L = 0;
        float R = dst_frame.size.width;
        float T = 0;
        float B = dst_frame.size.height;
        float N = (float)viewport.znear;
        float F = (float)viewport.zfar;
        float X = 1;
        float Y = 1;
        
        main_uniforms_t main_uniform =
        {
            .projection_matrix =
            {
                { (2.0f * X)/(R-L),   0.0f,                 0.0f,   0.0f },
                { 0.0f,               (2.0f * Y)/(T-B),     0.0f,   0.0f },
                { 0.0f,               0.0f,                 1/(F-N),   0.0f },
                { (R+L)/(L-R),        (T+B)/(B-T),          N/(F-N),   1.0f },
            }
        };
        
        MTL::RenderPassDescriptor *desc = MTL::RenderPassDescriptor::alloc()->init();
        desc->colorAttachments()->object(0)->setTexture(dst_texture);
        
        switch(operation)
        {
            case boden::graphic::compositing_operation_t::clear:
                desc->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
                desc->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0.0f, 0.0f, 0.0f, 0.0f));
                break;
                
            default:
                desc->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionLoad);
                desc->colorAttachments()->object(0)->setStoreAction(MTL::StoreActionStore);
                break;
        }

        MTL::RenderCommandEncoder *encoder = command_buffer->renderCommandEncoder(desc);
        desc->release();

        encoder->pushDebugGroup(NS::String::string(
            "Render to texture",
            NS::StringEncoding::UTF8StringEncoding));

        encoder->setCullMode(MTL::CullModeNone);
        encoder->setDepthStencilState(_depth_stencil.get());
        encoder->setViewport(viewport);
        encoder->setVertexBytes(&main_uniform, sizeof(main_uniform), 1);
        encoder->setVertexBuffer(vertex_buffer->get_buffer(), 0, 0);
        
        switch(operation)
        {
            case boden::graphic::compositing_operation_t::clear:
                encoder->setRenderPipelineState(_pipeline_default.get());
                break;
                
            case boden::graphic::compositing_operation_t::copy:
                encoder->setRenderPipelineState(_pipeline_premultiplied.get());
                break;
                
            case boden::graphic::compositing_operation_t::source_over:
                encoder->setRenderPipelineState(_pipeline_default.get());
                break;
                
            default:
                break;
        }
        
        for(auto &command : command_group.commands)
        {
            int32_t x = std::clamp<int32_t>(command.clip_rect.origin.x, 0, dst_frame.size.width);
            int32_t y = std::clamp<int32_t>(command.clip_rect.origin.y, 0, dst_frame.size.height);
            int32_t width = std::min<int32_t>(command.clip_rect.size.width, dst_frame.size.width - x);
            int32_t height = std::min<int32_t>(command.clip_rect.size.height, dst_frame.size.height - y);
            
            MTL::ScissorRect scissor_rect =
            {
                .x = (NS::UInteger)(x),
                .y = (NS::UInteger)(y),
                .width = (NS::UInteger)(width),
                .height = (NS::UInteger)(height)
            };
            encoder->setScissorRect(scissor_rect);
            
            if(command.texture_id)
            {
                auto src_texture = reinterpret_cast<const MTL::Texture *>(_texture_manager->get_gpu_texture_handle(command.texture_id));
                encoder->setFragmentTexture(src_texture, 0);
            }
            else
            {
                encoder->setFragmentTexture(_texture_default.get(), 0);
            }
            
            encoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangleStrip,
                                           command.count,
                                           MTL::IndexTypeUInt32,
                                           index_buffer->get_buffer(),
                                           command.index_buffer_offset * sizeof(boden::draw::index_t));
        }
        encoder->popDebugGroup();
        encoder->endEncoding();
    }
    
    command_buffer->addCompletedHandler([vertex_buffer, index_buffer](MTL::CommandBuffer* buffer)
    {
        buffer_manager.queue_reusable_buffer(vertex_buffer);
        buffer_manager.queue_reusable_buffer(index_buffer);
    });
    
    {
        boden::graphic::texture_id_t dst_tid = ctx.batch->command_groups[0].tid;
        boden::layout::rect_t dst_frame = ctx.batch->command_groups[0].frame;
        
        MTL::Texture *dst_texture = reinterpret_cast<MTL::Texture *>(_texture_manager->get_gpu_texture_handle(dst_tid));
        if(!dst_texture)
        {
            return;
        }

        MTL::Viewport viewport =
        {
            .originX = 0.0,
            .originY = 0.0,
            .width = (double)(dst_frame.size.width),
            .height = (double)(dst_frame.size.height),
            .znear = 0.0,
            .zfar = 1.0
        };
        
        float L = 0;
        float R = dst_frame.size.width;
        float T = 0;
        float B = dst_frame.size.height;
        float N = (float)viewport.znear;
        float F = (float)viewport.zfar;
        float X = 1;
        float Y = 1;
        
        main_uniforms_t main_uniform =
        {
            .projection_matrix =
            {
                { (2.0f * X)/(R-L),   0.0f,                 0.0f,   0.0f },
                { 0.0f,               (2.0f * Y)/(T-B),     0.0f,   0.0f },
                { 0.0f,               0.0f,                 1/(F-N),   0.0f },
                { (R+L)/(L-R),        (T+B)/(B-T),          N/(F-N),   1.0f },
            }
        };
        
        MTL::RenderPassDescriptor *desc = MTL::RenderPassDescriptor::alloc()->init();
        desc->colorAttachments()->object(0)->setTexture(dst_texture);
        desc->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionLoad);
        desc->colorAttachments()->object(0)->setStoreAction(MTL::StoreActionStore);
        
        MTL::RenderCommandEncoder *encoder = command_buffer->renderCommandEncoder(desc);
        desc->release();
        
        encoder->pushDebugGroup(NS::String::string(
                                                   "Render to root texture",
                                                   NS::StringEncoding::UTF8StringEncoding));
        
        encoder->setCullMode(MTL::CullModeNone);
        encoder->setDepthStencilState(_depth_stencil.get());
        encoder->setRenderPipelineState(_pipeline_default.get());
        encoder->setViewport(viewport);
        encoder->setVertexBytes(&main_uniform, sizeof(main_uniform), 1);
        
        for(auto &command_group : ctx.batch->command_groups)
        {
            if(command_group.tid == dst_tid)
            {
                continue;
            }
            
            auto src_texture = reinterpret_cast<MTL::Texture *>(_texture_manager->get_gpu_texture_handle(command_group.tid));
            
            float x = command_group.frame.origin.x;
            float y = command_group.frame.origin.y;
            float w = command_group.frame.size.width;
            float h = command_group.frame.size.height;
            
            MTL::ScissorRect scissor_rect =
            {
                .x = (NS::UInteger)(x),
                .y = (NS::UInteger)(y),
                .width = (NS::UInteger)(w),
                .height = (NS::UInteger)(h)
            };

            boden::draw::vertex_t quad[4] =
            {
                { {x,     y},     {0.0f, 0.0f}, 0xFFFFFFFF },
                { {x + w, y},     {1.0f, 0.0f}, 0xFFFFFFFF },
                { {x,     y + h}, {0.0f, 1.0f}, 0xFFFFFFFF },
                { {x + w, y + h}, {1.0f, 1.0f}, 0xFFFFFFFF }
            };
            
            encoder->setScissorRect(scissor_rect);
            encoder->setVertexBytes(quad, sizeof(quad), 0);
            encoder->setFragmentTexture(src_texture, 0);
            encoder->drawPrimitives(MTL::PrimitiveTypeTriangleStrip,
                                    static_cast<NS::UInteger>(0),
                                    static_cast<NS::UInteger>(4));
        }
        
        encoder->popDebugGroup();
        encoder->endEncoding();
    }

    CA::MetalDrawable *surface = reinterpret_cast<CA::MetalDrawable *>(ctx.surface_handle);
    {
        boden::graphic::texture_id_t srd_tid = ctx.batch->command_groups[0].tid;
        boden::layout::rect_t src_frame = ctx.batch->command_groups[0].frame;

        MTL::Texture *src_texture = reinterpret_cast<MTL::Texture *>(_texture_manager->get_gpu_texture_handle(srd_tid));
        if(!src_texture)
        {
            return;
        }

        MTL::Viewport viewport =
        {
            .originX = 0.0,
            .originY = 0.0,
            .width = (double)(ctx.display_size.width * ctx.display_scale.x),
            .height = (double)(ctx.display_size.height * ctx.display_scale.y),
            .znear = 0.0,
            .zfar = 1.0
        };
        
        float L = 0;
        float R = ctx.display_size.width * ctx.display_scale.x;
        float T = 0;
        float B = ctx.display_size.height * ctx.display_scale.y;
        float N = (float)viewport.znear;
        float F = (float)viewport.zfar;
        float X = ctx.display_scale.x;
        float Y = ctx.display_scale.y;
        
        main_uniforms_t main_uniform =
        {
            .projection_matrix =
            {
                { (2.0f * X)/(R-L),   0.0f,                 0.0f,   0.0f },
                { 0.0f,               (2.0f * Y)/(T-B),     0.0f,   0.0f },
                { 0.0f,               0.0f,                 1/(F-N),   0.0f },
                { (R+L)/(L-R),        (T+B)/(B-T),          N/(F-N),   1.0f },
            }
        };
        
        float x = src_frame.origin.x;
        float y = src_frame.origin.y;
        float w = src_frame.size.width;
        float h = src_frame.size.height;

        boden::draw::vertex_t quad[4] =
        {
            { {x,     y},     {0.0f, 0.0f}, 0xFFFFFFFF },
            { {x + w, y},     {1.0f, 0.0f}, 0xFFFFFFFF },
            { {x,     y + h}, {0.0f, 1.0f}, 0xFFFFFFFF },
            { {x + w, y + h}, {1.0f, 1.0f}, 0xFFFFFFFF }
        };
        
        MTL::RenderPassDescriptor *desc = MTL::RenderPassDescriptor::alloc()->init();
        desc->colorAttachments()->object(0)->setTexture(surface->texture());
        desc->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
        desc->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(1.0f, 0.0f, 1.0f, 1.0f));
        
        MTL::RenderCommandEncoder *encoder = command_buffer->renderCommandEncoder(desc);
        desc->release();
        
        encoder->pushDebugGroup(NS::String::string(
                                                   "Render to surface",
                                                   NS::StringEncoding::UTF8StringEncoding));
        
        encoder->setCullMode(MTL::CullModeNone);
        encoder->setDepthStencilState(_depth_stencil.get());
        encoder->setRenderPipelineState(_pipeline_default.get());
        encoder->setViewport(viewport);
        encoder->setVertexBytes(quad, sizeof(quad), 0);
        encoder->setVertexBytes(&main_uniform, sizeof(main_uniform), 1);
        encoder->setFragmentTexture(src_texture, 0);
        encoder->drawPrimitives(MTL::PrimitiveTypeTriangleStrip,
                                    static_cast<NS::UInteger>(0),
                                    static_cast<NS::UInteger>(4));
        
        encoder->popDebugGroup();
        encoder->endEncoding();
    }
    
    command_buffer->presentDrawable(surface);
    command_buffer->commit();
}

void mtl_renderer_t::setup_depth_stencil()
{
    MTL::DepthStencilDescriptor *desc = MTL::DepthStencilDescriptor::alloc()->init();
    desc->setDepthCompareFunction(MTL::CompareFunctionAlways);
    desc->setDepthWriteEnabled(false);
    _depth_stencil.reset(_device->newDepthStencilState(desc));
}

void mtl_renderer_t::setup_texture()
{
    MTL::TextureDescriptor *desc = MTL::TextureDescriptor::alloc()->init();
    desc->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
    desc->setWidth(1);
    desc->setHeight(1);
    desc->setUsage(MTL::TextureUsageShaderRead);
    _texture_default.reset(_device->newTexture(desc));
    uint8_t whitePixel[4] = {255, 255, 255, 255};
    MTL::Region region = MTL::Region::Make3D(0, 0, 0, 1, 1, 1);
    _texture_default->replaceRegion(region, 0, whitePixel, 4);
}

void mtl_renderer_t::setup_pipeline()
{
    NS::String *source = NS::String::alloc()->init(R"(
    #include <metal_stdlib>
    using namespace metal;
    
    struct main_uniforms_t
    {
        float4x4 projection_matrix;
    };
    
    struct main_vertex_in_t 
    {
        float2 position [[attribute(0)]];
        float2 tex_coords [[attribute(1)]];
        uchar4 color [[attribute(2)]];
    };
    
    struct main_vertex_out_t 
    {
        float4 position [[position]];
        float2 tex_coords;
        float4 color;
    };
    
    vertex main_vertex_out_t main_vertex(main_vertex_in_t in [[stage_in]],
                                         constant main_uniforms_t &uniforms [[buffer(1)]]) 
    {
        main_vertex_out_t out;
        out.position = uniforms.projection_matrix * float4(in.position, 0, 1);
        out.tex_coords = in.tex_coords;
        out.color = float4(in.color) / float4(255.0);
        return out;
    }

    fragment half4 main_fragment(main_vertex_out_t in [[stage_in]],
                                 texture2d<half, access::sample> texture [[texture(0)]]) 
    {
        constexpr sampler linear_sampler(coord::normalized, min_filter::linear, mag_filter::linear, mip_filter::linear);
        half4 tex_color = texture.sample(linear_sampler, in.tex_coords);
        return half4(in.color) * tex_color;
    }
    
    struct grid_uniforms_t 
    {
        float2 pan;     
        float zoom;     
        float grid_spacing; 
        float2 screen_size;
    };

    vertex float4 grid_vertex(uint vertex_id [[vertex_id]])
    {
        float2 pos[3] = 
        {
            float2(-1.0, -1.0),
            float2(3.0, -1.0),
            float2(-1.0, 3.0)
        };
        return float4(pos[vertex_id], 0.0, 1.0);
    }

    fragment half4 grid_fragment(float4 position [[position]],
                                 constant grid_uniforms_t &uniforms [[buffer(0)]])
    {
        float2 grid_coord = (position.xy / uniforms.screen_size) * uniforms.zoom + uniforms.pan;

        float2 grid_mod = fmod(grid_coord, uniforms.grid_spacing);
        float2 grid_dist = min(grid_mod, uniforms.grid_spacing - grid_mod);

        float minor = smoothstep(0.5, 0.0, min(grid_dist.x, grid_dist.y));
        float major = step(0.0, fmod(grid_coord.x, uniforms.grid_spacing * 10.0)) *
                      step(0.0, fmod(grid_coord.y, uniforms.grid_spacing * 10.0));

        float intensity = max(minor * 0.3, major * 1.0);

        return half4(intensity, intensity, intensity, 1.0);
    }
    )", NS::StringEncoding::UTF8StringEncoding);
    
    NS::Error *error = nullptr;
    MTL::Library *library = _device->newLibrary(source, nullptr, &error);
    if(library == nullptr)
    {
        std::cout << "Error: failed to create Metal library: " << error << std::endl;
        return;
    }
    
    MTL::Function *vertex_func = library->newFunction(NS::String::string("main_vertex", NS::UTF8StringEncoding));
    MTL::Function *fragment_func = library->newFunction(NS::String::string("main_fragment", NS::UTF8StringEncoding));
    if(vertex_func == nullptr || fragment_func == nullptr)
    {
        std::cout << "Error: failed to find Metal shader functions in library: " << error << std::endl;
        if(vertex_func) vertex_func->release();
        if(fragment_func) fragment_func->release();
        library->release();
        return;
    }
    
    MTL::VertexDescriptor *vertex_desc = MTL::VertexDescriptor::alloc()->init();
    vertex_desc->attributes()->object(0)->setOffset(offsetof(boden::draw::vertex_t, position));
    vertex_desc->attributes()->object(0)->setFormat(MTL::VertexFormatFloat2);
    vertex_desc->attributes()->object(0)->setBufferIndex(0);
    vertex_desc->attributes()->object(1)->setOffset(offsetof(boden::draw::vertex_t, uv));
    vertex_desc->attributes()->object(1)->setFormat(MTL::VertexFormatFloat2);
    vertex_desc->attributes()->object(1)->setBufferIndex(0);
    vertex_desc->attributes()->object(2)->setOffset(offsetof(boden::draw::vertex_t, color));
    vertex_desc->attributes()->object(2)->setFormat(MTL::VertexFormatUChar4);
    vertex_desc->attributes()->object(2)->setBufferIndex(0);
    vertex_desc->layouts()->object(0)->setStepRate(1);
    vertex_desc->layouts()->object(0)->setStepFunction(MTL::VertexStepFunctionPerVertex);
    vertex_desc->layouts()->object(0)->setStride(sizeof(boden::draw::vertex_t));

    MTL::RenderPipelineDescriptor *pipeline_desc = MTL::RenderPipelineDescriptor::alloc()->init();
    pipeline_desc->setVertexFunction(vertex_func);
    pipeline_desc->setFragmentFunction(fragment_func);
    pipeline_desc->setVertexDescriptor(vertex_desc);
    pipeline_desc->setRasterSampleCount(1);
    pipeline_desc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
    pipeline_desc->colorAttachments()->object(0)->setBlendingEnabled(true);
    pipeline_desc->colorAttachments()->object(0)->setRgbBlendOperation(MTL::BlendOperationAdd);
    pipeline_desc->colorAttachments()->object(0)->setSourceRGBBlendFactor(MTL::BlendFactorSourceAlpha);
    pipeline_desc->colorAttachments()->object(0)->setDestinationRGBBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
    pipeline_desc->colorAttachments()->object(0)->setAlphaBlendOperation(MTL::BlendOperationAdd);
    pipeline_desc->colorAttachments()->object(0)->setSourceAlphaBlendFactor(MTL::BlendFactorOne);
    pipeline_desc->colorAttachments()->object(0)->setDestinationAlphaBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
    pipeline_desc->setDepthAttachmentPixelFormat(MTL::PixelFormatInvalid);
    pipeline_desc->setStencilAttachmentPixelFormat(MTL::PixelFormatInvalid);
    
    _pipeline_default.reset(_device->newRenderPipelineState(pipeline_desc, &error));
    if(_pipeline_default == nullptr)
    {
        std::cout << "Error: failed to create Metal pipeline state: " << error << std::endl;
        if(vertex_desc) vertex_desc->release();
        if(pipeline_desc) pipeline_desc->release();
        vertex_func->release();
        fragment_func->release();
        library->release();
        return;
    }
    
    pipeline_desc->colorAttachments()->object(0)->setSourceRGBBlendFactor(MTL::BlendFactorOne);
    pipeline_desc->colorAttachments()->object(0)->setDestinationRGBBlendFactor(MTL::BlendFactorZero);
    
    _pipeline_premultiplied.reset(_device->newRenderPipelineState(pipeline_desc, &error));
    if(_pipeline_premultiplied == nullptr)
    {
        std::cout << "Error: failed to create Metal pipeline state: " << error << std::endl;
        if(vertex_desc) vertex_desc->release();
        if(pipeline_desc) pipeline_desc->release();
        vertex_func->release();
        fragment_func->release();
        library->release();
        return;
    }
    
    vertex_desc->release();
    pipeline_desc->release();
    vertex_func->release();
    fragment_func->release();
    library->release();
}

} // platform
