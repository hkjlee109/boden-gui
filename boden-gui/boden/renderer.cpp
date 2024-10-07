#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "renderer.hpp"

namespace boden {

renderer_t::renderer_t(
    CA::MetalDrawable* drawable,
    MTL::Device* device
) :
_drawable{drawable},
_device{device},
_commandQueue{_device->newCommandQueue()} {

}

renderer_t::~renderer_t() {
    _commandQueue->release();
}

void renderer_t::draw_rect() {
    MTL::CommandBuffer* commandBuffer = _commandQueue->commandBuffer();
    MTL::RenderPassDescriptor* descriptor = MTL::RenderPassDescriptor::alloc()->init();
    
    descriptor->colorAttachments()->object(0)->setTexture(_drawable->texture());
    descriptor->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
    descriptor->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0.0, 1.0, 0.0, 1.0));
    
    MTL::RenderCommandEncoder* encoder = commandBuffer->renderCommandEncoder(descriptor);
    encoder->endEncoding();
    commandBuffer->presentDrawable(_drawable);
    commandBuffer->commit();
    
    descriptor->release();
}

} // boden
