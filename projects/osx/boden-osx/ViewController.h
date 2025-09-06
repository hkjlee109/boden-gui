#import "platform/osx_queue.hpp"

#import <Cocoa/Cocoa.h>
#import <MetalKit/MetalKit.h>

@interface ViewController : NSViewController

- (instancetype)initWithDevice:(id<MTLDevice>)device
                         queue:(platform::osx_queue_t *)queue;

- (MTKView *)mtkView;

@end
