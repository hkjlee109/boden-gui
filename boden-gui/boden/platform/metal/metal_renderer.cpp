#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "metal_renderer.hpp"
#include <simd/simd.h>

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
}

metal_renderer_t::~metal_renderer_t()
{
    _commandQueue->release();
}

void metal_renderer_t::draw_rect()
{
    MTL::CommandBuffer* commandBuffer = _commandQueue->commandBuffer();
    MTL::RenderPassDescriptor* descriptor = MTL::RenderPassDescriptor::alloc()->init();
    
    descriptor->colorAttachments()->object(0)->setTexture(_drawable->texture());
    descriptor->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
    descriptor->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0.0, 1.0, 0.0, 1.0));
    
    MTL::RenderCommandEncoder* encoder = commandBuffer->renderCommandEncoder(descriptor);
    
    typedef struct {
        vector_float4 position;
        vector_float4 color;
    } MBEVertex;
    
    MBEVertex vertices[] = {
        { .position = {  0.0,  0.5, 0, 1 }, .color = { 1, 0, 0, 1 } },
        { .position = { -0.5, -0.5, 0, 1 }, .color = { 0, 1, 0, 1 } },
        { .position = {  0.5, -0.5, 0, 1 }, .color = { 0, 0, 1, 1 } },
    };
    
    MTL::Buffer* vertexBuffer = _device->newBuffer(vertices, sizeof(vertices), MTL::ResourceStorageModeShared);
    
    encoder->setRenderPipelineState(_pipeline);
    encoder->setVertexBuffer(vertexBuffer, 0, 0);
    encoder->drawPrimitives(MTL::PrimitiveTypeTriangleStrip, 0, 3, 1);
    
    encoder->endEncoding();
    commandBuffer->presentDrawable(_drawable);
    commandBuffer->commit();
    
    descriptor->release();
    vertexBuffer->release();
}

void metal_renderer_t::setup_pipeline()
{
    NS::String *source = NS::String::alloc()->init(R"(
    #include <metal_stdlib>
    using namespace metal;
    
    struct Vertex {
        float4 position [[position]];
        float4 color;
    };
    
    vertex Vertex vertex_main(
        const device Vertex* vertices [[buffer(0)]],
        uint vid [[vertex_id]]
    ) {
        return vertices[vid];
    }
    
    fragment float4 fragment_main(
        Vertex inVertex [[stage_in]]
    ) {
        return inVertex.color;
    } 
    )", NS::StringEncoding::UTF8StringEncoding);
    NS::Error* error = nullptr;
    MTL::Library* library = _device->newLibrary(source, nullptr, &error);
    
    MTL::Function* vertexFunction = library->newFunction(NS::String::string("vertex_main", NS::UTF8StringEncoding));
    MTL::Function* fragmentFunction = library->newFunction(NS::String::string("fragment_main", NS::UTF8StringEncoding));
    
    MTL::RenderPipelineDescriptor* pipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    pipelineDescriptor->setVertexFunction(vertexFunction);
    pipelineDescriptor->setFragmentFunction(fragmentFunction);
    pipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
    
    _pipeline = _device->newRenderPipelineState(pipelineDescriptor, &error);
    
    pipelineDescriptor->release();
    vertexFunction->release();
    fragmentFunction->release();
    library->release();
}

} // metal
} // platform
} // boden
