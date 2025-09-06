#import "AppDelegate.h"
#import "ViewController.h"

#import "platform/osx_backend.hpp"
#import "platform/osx_queue.hpp"

#import <MetalKit/MetalKit.h>

@interface AppDelegate ()

@end

@implementation AppDelegate {
    NSWindow *_window;
    platform::osx_queue_t _queue;
    std::unique_ptr<platform::osx_backend_t> _backend;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    
    _window = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, 640, 480)
                                         styleMask:NSWindowStyleMaskTitled|NSWindowStyleMaskClosable|NSWindowStyleMaskResizable
                                           backing:NSBackingStoreBuffered
                                             defer:YES];
    
    ViewController *controller = [[ViewController alloc] initWithDevice:device
                                                                  queue:&_queue];

    _window.contentViewController = controller;

    [_window center];
    [_window orderFront:self];
    
    _backend = std::make_unique<platform::osx_backend_t>(_queue);
    _backend->start();
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
}

- (BOOL)applicationSupportsSecureRestorableState:(NSApplication *)app {
    return YES;
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender {
    return YES;
}

@end
