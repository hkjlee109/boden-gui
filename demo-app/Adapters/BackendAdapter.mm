#import "BackendAdapter.h"

#include <boden/backend.hpp>
#include <memory>

@implementation BackendAdapter {
    std::unique_ptr<boden::backend_t> _backend;
}

- (instancetype)initWithDrawable:(id<CAMetalDrawable>)drawable
                          device:(id<MTLDevice>)device {
    self = [self init];
    if(self) {
        _backend = std::make_unique<boden::backend_t>(
            (__bridge CA::MetalDrawable*)drawable,
            (__bridge MTL::Device*)device
        );
    }
    return self;
}

- (void)draw {
    _backend->draw();
}

@end
