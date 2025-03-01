#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "metal_renderer.hpp"

#include <boden/draw/draw_vertex.hpp>
#include <simd/simd.h>
#include <iostream>

namespace platform {

metal_renderer_t::metal_renderer_t(CA::MetalDrawable* drawable,
                                   MTL::Device* device)
    : renderer_t(),
      _drawable{drawable},
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

void metal_renderer_t::begin_paint()
{
}

void metal_renderer_t::end_paint()
{
    MTL::CommandBuffer *command_buffer = _command_queue->commandBuffer();
    MTL::RenderPassDescriptor *descriptor = MTL::RenderPassDescriptor::alloc()->init();
    
    descriptor->colorAttachments()->object(0)->setTexture(_drawable->texture());
    descriptor->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
    descriptor->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0.45f, 0.55f, 0.60f, 1.00f));

    MTL::RenderCommandEncoder *encoder = command_buffer->renderCommandEncoder(descriptor);
    
    boden::draw::draw_vertex_t vertices[] = {
        { .position = {  0,  0 }, .uv = { 0, 0 }, .color = 0xFF0000FF },
        { .position = {  0, 300 }, .uv = { 0, 0 }, .color = 0xFF00FF00 },
        { .position = { 1200, 0 }, .uv = { 0, 0 }, .color = 0xFFFF0000 },
        { .position = { 1200, 300 }, .uv = { 0, 0 }, .color = 0xFF00FFFF },
    };

    MTL::Buffer *vertex_buffer = _device->newBuffer(vertices, sizeof(vertices), MTL::ResourceStorageModeShared);
    
    
    
    
    
    
    
    encoder->setCullMode(MTL::CullModeNone);
    encoder->setDepthStencilState(_depth_stencil_state.get());

    MTL::Viewport viewport =
    {
        .originX = 0.0,
        .originY = 0.0,
        .width = (double)(1200),
        .height = (double)(720),
        .znear = 0.0,
        .zfar = 1.0
    };
    encoder->setViewport(viewport);
    
    float L = 0;
    float R = 1200;
    float T = 0;
    float B = 720;
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
    
    
    
    MTL::ScissorRect scissorRect =
    {
        .x = 0,
        .y = 0,
        .width = 640,
        .height = 480
    };
    encoder->setScissorRect(scissorRect);

    encoder->setFragmentTexture(_texture.get(), 0);
    
    encoder->setRenderPipelineState(_render_pipeline.get());
    encoder->setVertexBuffer(vertex_buffer, 0, 0);
    encoder->drawPrimitives(MTL::PrimitiveTypeTriangleStrip, 0, 4, 1);
    
    encoder->endEncoding();
    command_buffer->presentDrawable(_drawable);
    command_buffer->commit();
    
    descriptor->release();
    vertex_buffer->release();
}

void metal_renderer_t::draw_rect()
{
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
    
    MTL::Function *vertexFunction = library->newFunction(NS::String::string("vertex_main", NS::UTF8StringEncoding));
    MTL::Function *fragmentFunction = library->newFunction(NS::String::string("fragment_main", NS::UTF8StringEncoding));
    if(vertexFunction == nullptr || fragmentFunction == nullptr)
    {
        std::cout << "Error: failed to find Metal shader functions in library: " << error << std::endl;
        if(vertexFunction) vertexFunction->release();
        if(fragmentFunction) fragmentFunction->release();
        library->release();
        return;
    }
    
    MTL::VertexDescriptor *vertexDescriptor = MTL::VertexDescriptor::alloc()->init();
    vertexDescriptor->attributes()->object(0)->setOffset(offsetof(boden::draw::draw_vertex_t, position));
    vertexDescriptor->attributes()->object(0)->setFormat(MTL::VertexFormatFloat2);
    vertexDescriptor->attributes()->object(0)->setBufferIndex(0);
    vertexDescriptor->attributes()->object(1)->setOffset(offsetof(boden::draw::draw_vertex_t, uv));
    vertexDescriptor->attributes()->object(1)->setFormat(MTL::VertexFormatFloat2);
    vertexDescriptor->attributes()->object(1)->setBufferIndex(0);
    vertexDescriptor->attributes()->object(2)->setOffset(offsetof(boden::draw::draw_vertex_t, color));
    vertexDescriptor->attributes()->object(2)->setFormat(MTL::VertexFormatUChar4);
    vertexDescriptor->attributes()->object(2)->setBufferIndex(0);
    vertexDescriptor->layouts()->object(0)->setStepRate(1);
    vertexDescriptor->layouts()->object(0)->setStepFunction(MTL::VertexStepFunctionPerVertex);
    vertexDescriptor->layouts()->object(0)->setStride(sizeof(boden::draw::draw_vertex_t));

    MTL::RenderPipelineDescriptor *pipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    pipelineDescriptor->setVertexFunction(vertexFunction);
    pipelineDescriptor->setFragmentFunction(fragmentFunction);
    pipelineDescriptor->setVertexDescriptor(vertexDescriptor);
    pipelineDescriptor->setRasterSampleCount(1);
    pipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
    
    pipelineDescriptor->colorAttachments()->object(0)->setBlendingEnabled(true);
    pipelineDescriptor->colorAttachments()->object(0)->setRgbBlendOperation(MTL::BlendOperationAdd);
    pipelineDescriptor->colorAttachments()->object(0)->setSourceRGBBlendFactor(MTL::BlendFactorSourceAlpha);
    pipelineDescriptor->colorAttachments()->object(0)->setDestinationRGBBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
    pipelineDescriptor->colorAttachments()->object(0)->setAlphaBlendOperation(MTL::BlendOperationAdd);
    pipelineDescriptor->colorAttachments()->object(0)->setSourceAlphaBlendFactor(MTL::BlendFactorOne);
    pipelineDescriptor->colorAttachments()->object(0)->setDestinationAlphaBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
    pipelineDescriptor->setDepthAttachmentPixelFormat(MTL::PixelFormatInvalid);
    pipelineDescriptor->setStencilAttachmentPixelFormat(MTL::PixelFormatInvalid);
    
    _render_pipeline.reset(_device->newRenderPipelineState(pipelineDescriptor, &error));
    if(_render_pipeline == nullptr)
    {
        std::cout << "Error: failed to create Metal pipeline state: " << error << std::endl;
        if(vertexDescriptor) vertexDescriptor->release();
        if(pipelineDescriptor) pipelineDescriptor->release();
        vertexFunction->release();
        fragmentFunction->release();
        library->release();
        return;
    }
    
    vertexDescriptor->release();
    pipelineDescriptor->release();
    vertexFunction->release();
    fragmentFunction->release();
    library->release();
}

} // platform
