#import "AppDelegate.h"
#import "WindowController.h"

@interface AppDelegate ()

@end

@implementation AppDelegate {
    WindowController* _windowController;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    if(_windowController == nil) {
        _windowController = [WindowController new];
    }
    
    [_windowController.window orderFront:self];
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
}


- (BOOL)applicationSupportsSecureRestorableState:(NSApplication *)app {
    return YES;
}

@end
