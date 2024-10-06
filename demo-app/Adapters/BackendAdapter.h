#import <MetalKit/MetalKit.h>
#import <Metal/MTLDevice.h>

@interface BackendAdapter: NSObject 

- (instancetype)initWithDrawable:(id<CAMetalDrawable>)drawable
                          device:(id<MTLDevice>)device;

@end
