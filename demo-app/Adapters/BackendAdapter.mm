#import "BackendAdapter.h"

#include <boden/backend.hpp>
#include <boden/renderer.hpp>
#include <boden/platform/metal/metal_renderer.hpp>
#include <memory>

using metal_renderer_t = boden::platform::metal::metal_renderer_t;

@implementation BackendAdapter {
    std::unique_ptr<boden::backend_t> _backend;
}

- (instancetype)initWithDrawable:(id<CAMetalDrawable>)drawable
                          device:(id<MTLDevice>)device {
    self = [self init];
    if(self) {
        std::unique_ptr<boden::renderer_t> renderer = std::make_unique<metal_renderer_t>(
            (__bridge CA::MetalDrawable*)drawable,
            (__bridge MTL::Device*)device
        );

        _backend = std::make_unique<boden::backend_t>(
            std::move(renderer)
        );
    }
    return self;
}

- (void)draw {
    _backend->draw();
}

@end
