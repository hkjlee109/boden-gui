#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "metal_renderer.hpp"

#include <boden/draw/draw_vertex.hpp>
#include <simd/simd.h>
#include <iostream>

namespace boden {
namespace platform {
namespace metal {

metal_renderer_t::metal_renderer_t(
    CA::MetalDrawable* drawable,
    MTL::Device* device
) :
    renderer_t(),
    _drawable{drawable},
    _device{device},
    _commandQueue{_device->newCommandQueue()}
{
    setup_pipeline();
    setup_depth_stencil();
}

metal_renderer_t::~metal_renderer_t()
{
    if(_drawable)           _drawable->release();
    if(_device)             _device->release();
    if(_commandQueue)       _commandQueue->release();
    if(_pipeline)           _pipeline->release();
    if(_depthStencilState)  _depthStencilState->release();
}

void metal_renderer_t::begin_paint()
{
}

void metal_renderer_t::end_paint()
{
    MTL::CommandBuffer *commandBuffer = _commandQueue->commandBuffer();
    MTL::RenderPassDescriptor *descriptor = MTL::RenderPassDescriptor::alloc()->init();
    
    descriptor->colorAttachments()->object(0)->setTexture(_drawable->texture());
    descriptor->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
    descriptor->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0.0, 0.0, 1.0, 1.0));

    MTL::RenderCommandEncoder *encoder = commandBuffer->renderCommandEncoder(descriptor);

    boden::draw::draw_vertex_t vertices[] = {
        { .position = {  600,  0 }, .uv = { 1.0, 1.0 }, .color = 0 },
        { .position = {  0, 300 }, .uv = { 1.0, 1.0 }, .color = 0 },
        { .position = { 1200, 300 }, .uv = { 1.0, 1.0 }, .color = 0 },
    };
    
    MTL::Buffer *vertexBuffer = _device->newBuffer(vertices, sizeof(vertices), MTL::ResourceStorageModeShared);
    
    
    
    
    
    
    
    encoder->setCullMode(MTL::CullModeNone);
    encoder->setDepthStencilState(_depthStencilState);

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
        .width = 1200,
        .height = 720
    };
    encoder->setScissorRect(scissorRect);

    
    encoder->setRenderPipelineState(_pipeline);
    encoder->setVertexBuffer(vertexBuffer, 0, 0);
    encoder->drawPrimitives(MTL::PrimitiveTypeTriangle, 0, 3, 1);
    
    encoder->endEncoding();
    commandBuffer->presentDrawable(_drawable);
    commandBuffer->commit();
    
    descriptor->release();
    vertexBuffer->release();
}

void metal_renderer_t::draw_rect()
{
}

void metal_renderer_t::setup_depth_stencil()
{
    MTL::DepthStencilDescriptor *depthStencilDescriptor = MTL::DepthStencilDescriptor::alloc()->init();
    depthStencilDescriptor->setDepthCompareFunction(MTL::CompareFunctionAlways);
    depthStencilDescriptor->setDepthWriteEnabled(false);
    _depthStencilState = _device->newDepthStencilState(depthStencilDescriptor);
}

void metal_renderer_t::setup_pipeline()
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
    pipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
    
    _pipeline = _device->newRenderPipelineState(pipelineDescriptor, &error);
    if(_pipeline == nullptr)
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

} // metal
} // platform
} // boden
