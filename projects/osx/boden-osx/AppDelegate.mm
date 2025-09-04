#import "AppDelegate.h"
#import "ViewController.h"

#import "platform/osx_backend.hpp"

@interface AppDelegate ()

@end

@implementation AppDelegate {
    NSWindow *_window;
    platform::osx_backend_t _backend;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    _window = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, 640, 480)
                                         styleMask:NSWindowStyleMaskTitled|NSWindowStyleMaskClosable|NSWindowStyleMaskResizable
                                           backing:NSBackingStoreBuffered
                                             defer:YES];
    _window.contentViewController = [ViewController new];
    [_window center];
    [_window orderFront:self];
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
