#include "metal_renderer.hpp"

#include <boden/draw/vertex.hpp>
#include <simd/simd.h>
#include <iostream>

namespace platform {

metal_renderer_t::metal_renderer_t(MTL::Device* device)
    : boden::renderer_t(),
      _device{device},
      _command_queue{nullptr, [](MTL::CommandQueue *ptr) { if (ptr) ptr->release(); }},
      _render_pipeline{nullptr, [](MTL::RenderPipelineState *ptr) { if (ptr) ptr->release(); }},
      _depth_stencil_state{nullptr, [](MTL::DepthStencilState *ptr) { if (ptr) ptr->release(); }},
      _texture{nullptr, [](MTL::Texture *ptr) { if (ptr) ptr->release(); }}
{
    _command_queue.reset(_device->newCommandQueue());
    
    setup_render_pipeline();
    setup_depth_stencil_state();
    setup_default_texture();
}

metal_renderer_t::~metal_renderer_t()
{
}

void metal_renderer_t::begin_draw(boden::context_t &ctx)
{
    boden::renderer_t::begin_draw(ctx);
}

void metal_renderer_t::end_draw(boden::context_t &ctx)
{
    boden::renderer_t::end_draw(ctx);
    
    CA::MetalDrawable *surface = reinterpret_cast<CA::MetalDrawable *>(ctx.surface_handle);
    
    MTL::CommandBuffer *command_buffer = _command_queue->commandBuffer();
    MTL::RenderPassDescriptor *descriptor = MTL::RenderPassDescriptor::alloc()->init();
    
    descriptor->colorAttachments()->object(0)->setTexture(surface->texture());
    descriptor->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
    descriptor->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0.45f, 0.55f, 0.60f, 1.00f));

    MTL::RenderCommandEncoder *encoder = command_buffer->renderCommandEncoder(descriptor);
    
    MTL::Buffer *vertex_buffer = _device->newBuffer(builder.vertices.data(),
                                                    builder.vertices.size() * sizeof(boden::draw::vertex_t),
                                                    MTL::ResourceStorageModeShared);
    
    encoder->setCullMode(MTL::CullModeNone);
    encoder->setDepthStencilState(_depth_stencil_state.get());

    MTL::Viewport viewport =
    {
        .originX = 0.0,
        .originY = 0.0,
        .width = (double)(ctx.display_size.width * ctx.display_scale.x),
        .height = (double)(ctx.display_size.height * ctx.display_scale.y),
        .znear = 0.0,
        .zfar = 1.0
    };
    encoder->setViewport(viewport);
    
    float L = 0;
    float R = ctx.display_size.width * ctx.display_scale.x;
    float T = 0;
    float B = ctx.display_size.height * ctx.display_scale.y;
    float N = (float)viewport.znear;
    float F = (float)viewport.zfar;
    const float ortho_projection[4][4] =
    {
        { 2.0f/(R-L),   0.0f,           0.0f,   0.0f },
        { 0.0f,         2.0f/(T-B),     0.0f,   0.0f },
        { 0.0f,         0.0f,        1/(F-N),   0.0f },
        { (R+L)/(L-R),  (T+B)/(B-T), N/(F-N),   1.0f },
    };

    encoder->setVertexBytes(&ortho_projection, sizeof(ortho_projection), 1);
    
    for(const boden::draw::command_t &command : builder.commands)
    {
        MTL::ScissorRect scissorRect =
        {
            .x = 0,
            .y = 0,
            .width = (NS::UInteger)(ctx.display_size.width * ctx.display_scale.x),
            .height = (NS::UInteger)(ctx.display_size.height * ctx.display_scale.y)
        };
        encoder->setScissorRect(scissorRect);
        
        if(command.texture_id) {
            encoder->setFragmentTexture(reinterpret_cast<MTL::Texture *>(command.texture_id), 0);
        } else {
            encoder->setFragmentTexture(_texture.get(), 0);
        }
        
        encoder->setRenderPipelineState(_render_pipeline.get());
        encoder->setVertexBuffer(vertex_buffer, 0, 0);
        encoder->drawPrimitives(MTL::PrimitiveTypeTriangleStrip, command.vertex_offset, command.vertex_count, 1);
    }
    
    encoder->endEncoding();
    command_buffer->presentDrawable(surface);
    command_buffer->commit();
    
    descriptor->release();
    vertex_buffer->release();
}

void metal_renderer_t::setup_depth_stencil_state()
{
    MTL::DepthStencilDescriptor *descriptor = MTL::DepthStencilDescriptor::alloc()->init();
    descriptor->setDepthCompareFunction(MTL::CompareFunctionAlways);
    descriptor->setDepthWriteEnabled(false);
    _depth_stencil_state.reset(_device->newDepthStencilState(descriptor));
}

void metal_renderer_t::setup_default_texture()
{
    MTL::TextureDescriptor *descriptor = MTL::TextureDescriptor::alloc()->init();
    descriptor->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    descriptor->setWidth(1);
    descriptor->setHeight(1);
    descriptor->setUsage(MTL::TextureUsageShaderRead);
    _texture.reset(_device->newTexture(descriptor));
    uint8_t whitePixel[4] = {255, 255, 255, 255};
    MTL::Region region = MTL::Region::Make3D(0, 0, 0, 1, 1, 1);
    _texture->replaceRegion(region, 0, whitePixel, 4);
}

void metal_renderer_t::setup_render_pipeline()
{
    NS::String *source = NS::String::alloc()->init(R"(
    #include <metal_stdlib>
    using namespace metal;
    
    struct Uniforms {
        float4x4 projectionMatrix;
    };
    
    struct VertexIn {
        float2 position  [[attribute(0)]];
        float2 texCoords [[attribute(1)]];
        uchar4 color     [[attribute(2)]];
    };
    
    struct VertexOut {
        float4 position [[position]];
        float2 texCoords;
        float4 color;
    };
    
    vertex VertexOut vertex_main(
        VertexIn in [[stage_in]],
        constant Uniforms &uniforms [[buffer(1)]]
    ) {
        VertexOut out;
        out.position = uniforms.projectionMatrix * float4(in.position, 0, 1);
        out.texCoords = in.texCoords;
        out.color = float4(in.color) / float4(255.0);
        return out;
    }

    fragment half4 fragment_main(
        VertexOut in [[stage_in]],
        texture2d<half, access::sample> texture [[texture(0)]]
    ) {
        constexpr sampler linearSampler(coord::normalized, min_filter::linear, mag_filter::linear, mip_filter::linear);
        half4 texColor = texture.sample(linearSampler, in.texCoords);
    
        return half4(in.color) * texColor;
    }
    )", NS::StringEncoding::UTF8StringEncoding);
    
    NS::Error *error = nullptr;
    MTL::Library *library = _device->newLibrary(source, nullptr, &error);
    if(library == nullptr)
    {
        std::cout << "Error: failed to create Metal library: " << error << std::endl;
        return;
    }
    
    MTL::Function *vertex_func = library->newFunction(NS::String::string("vertex_main", NS::UTF8StringEncoding));
    MTL::Function *fragment_func = library->newFunction(NS::String::string("fragment_main", NS::UTF8StringEncoding));
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
    
    _render_pipeline.reset(_device->newRenderPipelineState(pipeline_desc, &error));
    if(_render_pipeline == nullptr)
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
