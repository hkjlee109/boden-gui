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
    simd::float2 offset;
    float zoom;
    float major_spacing;
    float minor_spacing;
    simd::float2 screen_size;
};

mtl_renderer_t::mtl_renderer_t(MTL::Device *device)
    : boden::renderer_t(),
      _device{device},
      _command_queue{nullptr, [](MTL::CommandQueue *ptr) { if (ptr) ptr->release(); }},
      _depth_stencil{nullptr, [](MTL::DepthStencilState *ptr) { if (ptr) ptr->release(); }},
      _grid_pipeline{nullptr, [](MTL::RenderPipelineState *ptr) { if (ptr) ptr->release(); }},
      _default_pipeline{nullptr, [](MTL::RenderPipelineState *ptr) { if (ptr) ptr->release(); }},
      _premultiplied_pipeline{nullptr, [](MTL::RenderPipelineState *ptr) { if (ptr) ptr->release(); }},
      _root_texture{nullptr, [](MTL::Texture *ptr) { if (ptr) ptr->release(); }},
      _white_texture{nullptr, [](MTL::Texture *ptr) { if (ptr) ptr->release(); }}
{
    _command_queue.reset(_device->newCommandQueue());
    
    setup_depth_stencil();
    setup_pipeline();
    setup_white_texture();
}

mtl_renderer_t::~mtl_renderer_t()
{
}

void mtl_renderer_t::render(boden::context_t &ctx)
{
    boden::renderer_t::render(ctx);
    
    if(ctx.batch->command_groups.empty())
    {
        return;
    }
    
    _texture_manager->cleanup_unused_texture();
    
    MTL::CommandBuffer *command_buffer = _command_queue->commandBuffer();
    
    mtl_buffer_ref_t vertex_buffer;
    mtl_buffer_ref_t index_buffer;
    
    if(!ctx.batch->vertices.empty())
    {
        vertex_buffer = buffer_manager.dequeue_reusable_buffer(_device,
                                                               ctx.batch->vertices.size() * sizeof(boden::draw::vertex_t));
        memcpy((char*)vertex_buffer->get_buffer()->contents(),
               ctx.batch->vertices.data(),
               ctx.batch->vertices.size() * sizeof(boden::draw::vertex_t));
    }
    
    if(!ctx.batch->indices.empty())
    {
        index_buffer = buffer_manager.dequeue_reusable_buffer(_device,
                                                              ctx.batch->indices.size() * sizeof(boden::draw::index_t));
    
        memcpy((char*)index_buffer->get_buffer()->contents(),
               ctx.batch->indices.data(),
               ctx.batch->indices.size() * sizeof(boden::draw::index_t));
    }
    
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
            
            case boden::graphic::compositing_operation_t::custom_1:
            {
                uint32_t zoom = 0;
                float offset_x = 0;
                float offset_y = 0;

                std::size_t offset = 0;
                std::memcpy(&zoom, command_group.params + offset, sizeof(zoom));
                offset += sizeof(zoom);
                std::memcpy(&offset_x, command_group.params + offset, sizeof(float));
                offset += sizeof(float);
                std::memcpy(&offset_y, command_group.params + offset, sizeof(float));
                
                desc->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
                desc->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0.0f, 0.0f, 0.0f, 0.0f));
                MTL::RenderCommandEncoder *encoder = command_buffer->renderCommandEncoder(desc);
                desc->release();
                
                encoder->setRenderPipelineState(_grid_pipeline.get());
                
                grid_uniforms_t uniforms;
                uniforms.offset.x = offset_x;
                uniforms.offset.y = offset_y;
                uniforms.zoom = zoom / 100.0f;
                uniforms.major_spacing = 100;
                uniforms.minor_spacing = 25;
                uniforms.screen_size.x = ctx.display_size.width;
                uniforms.screen_size.y = ctx.display_size.height;

                encoder->setFragmentBytes(&uniforms, sizeof(uniforms), 0);
                encoder->drawPrimitives(MTL::PrimitiveTypeTriangle,
                                        (NS::UInteger)0,
                                        (NS::UInteger)6);
                encoder->endEncoding();
                continue;
            }

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
        if(vertex_buffer)
        {
            encoder->setVertexBuffer(vertex_buffer->get_buffer(), 0, 0);
        }
        
        switch(operation)
        {
            case boden::graphic::compositing_operation_t::clear:
                encoder->setRenderPipelineState(_default_pipeline.get());
                break;
                
            case boden::graphic::compositing_operation_t::copy:
                encoder->setRenderPipelineState(_premultiplied_pipeline.get());
                break;
                
            case boden::graphic::compositing_operation_t::source_over:
                encoder->setRenderPipelineState(_default_pipeline.get());
                break;
                
            default:
                break;
        }
        
        for(auto &command : command_group.commands)
        {
            float x = std::clamp<float>(command.clip_rect.origin.x, 0, dst_frame.size.width);
            float y = std::clamp<float>(command.clip_rect.origin.y, 0, dst_frame.size.height);
            float w = std::min<float>(command.clip_rect.size.width, dst_frame.size.width - x);
            float h = std::min<float>(command.clip_rect.size.height, dst_frame.size.height - y);
            
            MTL::ScissorRect scissor_rect =
            {
                .x = (NS::UInteger)(x),
                .y = (NS::UInteger)(y),
                .width = (NS::UInteger)(w),
                .height = (NS::UInteger)(h)
            };
            encoder->setScissorRect(scissor_rect);
            
            if(command.texture_id)
            {
                auto src_texture = reinterpret_cast<const MTL::Texture *>(_texture_manager->get_gpu_texture_handle(command.texture_id));
                encoder->setFragmentTexture(src_texture, 0);
            }
            else
            {
                encoder->setFragmentTexture(_white_texture.get(), 0);
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
    
    create_root_texture_if_needed(ctx.display_size);
    
    {
        MTL::Texture *dst_texture = _root_texture.get();
        if(!dst_texture)
        {
            return;
        }

        MTL::RenderPassDescriptor *desc = MTL::RenderPassDescriptor::alloc()->init();
        desc->colorAttachments()->object(0)->setTexture(dst_texture);
        desc->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
        desc->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0.95f, 0.95f, 0.95f, 1.0f));
        
        MTL::RenderCommandEncoder *encoder = command_buffer->renderCommandEncoder(desc);
        desc->release();

        MTL::Viewport viewport =
        {
            .originX = 0.0,
            .originY = 0.0,
            .width = (double)ctx.display_size.width,
            .height = (double)ctx.display_size.height,
            .znear = 0.0,
            .zfar = 1.0
        };
        
        float L = 0;
        float R = ctx.display_size.width;
        float T = 0;
        float B = ctx.display_size.height;
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

        encoder->pushDebugGroup(NS::String::string(
                                                   "Render to root texture",
                                                   NS::StringEncoding::UTF8StringEncoding));
        
        encoder->setCullMode(MTL::CullModeNone);
        encoder->setDepthStencilState(_depth_stencil.get());
        encoder->setRenderPipelineState(_premultiplied_pipeline.get());
        encoder->setViewport(viewport);
        encoder->setVertexBytes(&main_uniform, sizeof(main_uniform), 1);
        
        boden::graphic::texture_id_t current_tid = 0;
        for(auto &command_group : ctx.batch->command_groups)
        {
            if(current_tid == command_group.tid)
            {
                continue;
            }
            
            current_tid = command_group.tid;
            auto src_texture = reinterpret_cast<MTL::Texture *>(_texture_manager->get_gpu_texture_handle(current_tid));
            
            float x = std::clamp<float>(command_group.frame.origin.x, 0, ctx.display_size.width);
            float y = std::clamp<float>(command_group.frame.origin.y, 0, ctx.display_size.height);
            float w = std::min<float>(command_group.frame.size.width, ctx.display_size.width - x);
            float h = std::min<float>(command_group.frame.size.height, ctx.display_size.height - y);
            
            float left_cut   = (x - command_group.frame.origin.x) / command_group.frame.size.width;
            float top_cut    = (y - command_group.frame.origin.y) / command_group.frame.size.height;
            float right_cut  = 1.0f - ((x + w - command_group.frame.origin.x) / command_group.frame.size.width);
            float bottom_cut = 1.0f - ((y + h - command_group.frame.origin.y) / command_group.frame.size.height);
            
            float u0 = left_cut;
            float v0 = top_cut;
            float u1 = 1.0f - right_cut;
            float v1 = 1.0f - bottom_cut;
            
            MTL::ScissorRect scissor_rect =
            {
                .x = (NS::UInteger)(x),
                .y = (NS::UInteger)(y),
                .width = (NS::UInteger)(w),
                .height = (NS::UInteger)(h)
            };

            boden::draw::vertex_t quad[4] =
            {
                { {x,     y},     {u0, v0}, 0xFFFFFFFF },
                { {x + w, y},     {u1, v0}, 0xFFFFFFFF },
                { {x,     y + h}, {u0, v1}, 0xFFFFFFFF },
                { {x + w, y + h}, {u1, v1}, 0xFFFFFFFF }
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
        MTL::Texture *src_texture = _root_texture.get();
        if(!src_texture)
        {
            return;
        }
        
        MTL::RenderPassDescriptor *desc = MTL::RenderPassDescriptor::alloc()->init();
        desc->colorAttachments()->object(0)->setTexture(surface->texture());
        desc->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
        desc->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0.95f, 0.95f, 0.95f, 1.0f));
        
        MTL::RenderCommandEncoder *encoder = command_buffer->renderCommandEncoder(desc);
        desc->release();

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
        
        float x = 0;
        float y = 0;
        float w = ctx.display_size.width;
        float h = ctx.display_size.height;

        boden::draw::vertex_t quad[4] =
        {
            { {x,     y},     {0.0f, 0.0f}, 0xFFFFFFFF },
            { {x + w, y},     {1.0f, 0.0f}, 0xFFFFFFFF },
            { {x,     y + h}, {0.0f, 1.0f}, 0xFFFFFFFF },
            { {x + w, y + h}, {1.0f, 1.0f}, 0xFFFFFFFF }
        };
        
        encoder->pushDebugGroup(NS::String::string(
                                                   "Render to surface",
                                                   NS::StringEncoding::UTF8StringEncoding));
        
        encoder->setCullMode(MTL::CullModeNone);
        encoder->setDepthStencilState(_depth_stencil.get());
        encoder->setRenderPipelineState(_default_pipeline.get());
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
    desc->release();
}

void mtl_renderer_t::setup_pipeline()
{
    MTL::Library *library = _device->newDefaultLibrary();
    
    if(library == nullptr)
    {
        std::cout << "Error: failed to create Metal library." << std::endl;
        return;
    }
    
    create_default_pipeline(library);
    create_grid_pipeline(library);
    create_premultiplied_pipeline(library);
    
    library->release();
}

void mtl_renderer_t::setup_white_texture()
{
    MTL::TextureDescriptor *desc = MTL::TextureDescriptor::alloc()->init();
    desc->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
    desc->setWidth(1);
    desc->setHeight(1);
    desc->setUsage(MTL::TextureUsageShaderRead);
    _white_texture.reset(_device->newTexture(desc));
    desc->release();
    
    uint8_t whitePixel[4] = {255, 255, 255, 255};
    MTL::Region region = MTL::Region::Make3D(0, 0, 0, 1, 1, 1);
    _white_texture->replaceRegion(region, 0, whitePixel, 4);
}

void mtl_renderer_t::create_default_pipeline(MTL::Library *library)
{
    MTL::Function *vertex_func = library->newFunction(NS::String::string("main_vertex", NS::UTF8StringEncoding));
    MTL::Function *fragment_func = library->newFunction(NS::String::string("main_fragment", NS::UTF8StringEncoding));
    if(vertex_func == nullptr || fragment_func == nullptr)
    {
        std::cout << "Error: Unable to find Metal shader functions in library." << std::endl;
        if(vertex_func) vertex_func->release();
        if(fragment_func) fragment_func->release();
        return;
    }
    
    NS::Error *error = nullptr;
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
    vertex_func->release();
    pipeline_desc->setFragmentFunction(fragment_func);
    fragment_func->release();
    pipeline_desc->setVertexDescriptor(vertex_desc);
    vertex_desc->release();
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
    
    _default_pipeline.reset(_device->newRenderPipelineState(pipeline_desc, &error));
    pipeline_desc->release();
    
    if(_default_pipeline == nullptr)
    {
        std::cout << "Error: failed to create Metal pipeline state: " << error << std::endl;
        return;
    }
}

void mtl_renderer_t::create_grid_pipeline(MTL::Library *library)
{
    MTL::Function *vertex_func = library->newFunction(NS::String::string("grid_vertex", NS::UTF8StringEncoding));
    MTL::Function *fragment_func = library->newFunction(NS::String::string("grid_fragment", NS::UTF8StringEncoding));
    if(vertex_func == nullptr || fragment_func == nullptr)
    {
        std::cout << "Error: Unable to find Metal shader functions in library. " << std::endl;
        if(vertex_func) vertex_func->release();
        if(fragment_func) fragment_func->release();
        return;
    }
    
    NS::Error *error = nullptr;
    MTL::RenderPipelineDescriptor *pipeline_desc = MTL::RenderPipelineDescriptor::alloc()->init();
    pipeline_desc->setVertexFunction(vertex_func);
    pipeline_desc->setFragmentFunction(fragment_func);
    pipeline_desc->setVertexDescriptor(nullptr);
    pipeline_desc->setRasterSampleCount(1);
    pipeline_desc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
    pipeline_desc->colorAttachments()->object(0)->setBlendingEnabled(true);
    pipeline_desc->colorAttachments()->object(0)->setRgbBlendOperation(MTL::BlendOperationAdd);
    pipeline_desc->colorAttachments()->object(0)->setSourceRGBBlendFactor(MTL::BlendFactorSourceAlpha);
    pipeline_desc->colorAttachments()->object(0)->setDestinationRGBBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
    pipeline_desc->colorAttachments()->object(0)->setAlphaBlendOperation(MTL::BlendOperationAdd);
    pipeline_desc->colorAttachments()->object(0)-> setSourceAlphaBlendFactor(MTL::BlendFactorSourceAlpha);
    pipeline_desc->colorAttachments()->object(0)->setDestinationAlphaBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
   
    pipeline_desc->setDepthAttachmentPixelFormat(MTL::PixelFormatInvalid);
    pipeline_desc->setStencilAttachmentPixelFormat(MTL::PixelFormatInvalid);
    
    _grid_pipeline.reset(_device->newRenderPipelineState(pipeline_desc, &error));
    if(_grid_pipeline == nullptr)
    {
        std::cout << "Error: failed to create Metal pipeline state: " << error << std::endl;
        return;
    }
}

void mtl_renderer_t::create_premultiplied_pipeline(MTL::Library *library)
{
    MTL::Function *vertex_func = library->newFunction(NS::String::string("main_vertex", NS::UTF8StringEncoding));
    MTL::Function *fragment_func = library->newFunction(NS::String::string("main_fragment", NS::UTF8StringEncoding));
    if(vertex_func == nullptr || fragment_func == nullptr)
    {
        std::cout << "Error: Unable to find Metal shader functions in library." << std::endl;
        if(vertex_func) vertex_func->release();
        if(fragment_func) fragment_func->release();
        return;
    }
    
    NS::Error *error = nullptr;
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
    vertex_func->release();
    pipeline_desc->setFragmentFunction(fragment_func);
    fragment_func->release();
    pipeline_desc->setVertexDescriptor(vertex_desc);
    vertex_desc->release();
    pipeline_desc->setRasterSampleCount(1);
    pipeline_desc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
    pipeline_desc->colorAttachments()->object(0)->setBlendingEnabled(true);
    pipeline_desc->colorAttachments()->object(0)->setRgbBlendOperation(MTL::BlendOperationAdd);
    pipeline_desc->colorAttachments()->object(0)->setSourceRGBBlendFactor(MTL::BlendFactorOne);
    pipeline_desc->colorAttachments()->object(0)->setDestinationRGBBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
    pipeline_desc->colorAttachments()->object(0)->setAlphaBlendOperation(MTL::BlendOperationAdd);
    pipeline_desc->colorAttachments()->object(0)->setSourceAlphaBlendFactor(MTL::BlendFactorOne);
    pipeline_desc->colorAttachments()->object(0)->setDestinationAlphaBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
    pipeline_desc->setDepthAttachmentPixelFormat(MTL::PixelFormatInvalid);
    pipeline_desc->setStencilAttachmentPixelFormat(MTL::PixelFormatInvalid);
    
    _premultiplied_pipeline.reset(_device->newRenderPipelineState(pipeline_desc, &error));
    pipeline_desc->release();

    if(_premultiplied_pipeline == nullptr)
    {
        std::cout << "Error: failed to create Metal pipeline state: " << error << std::endl;
        return;
    }
}

void mtl_renderer_t::create_root_texture_if_needed(const boden::layout::size_t &size)
{
    if(_root_texture == nullptr ||
       _root_texture->width() != size.width ||
       _root_texture->height() != size.height)
    {
        MTL::TextureDescriptor *desc = MTL::TextureDescriptor::alloc()->init();
        desc->setTextureType(MTL::TextureType2D);
        desc->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
        desc->setWidth(size.width);
        desc->setHeight(size.height);
        desc->setUsage(MTL::TextureUsageRenderTarget | MTL::TextureUsageShaderRead);
        desc->setStorageMode(MTL::StorageModePrivate);
        _root_texture.reset(_device->newTexture(desc));
        desc->release();
    }
}

} // platform
