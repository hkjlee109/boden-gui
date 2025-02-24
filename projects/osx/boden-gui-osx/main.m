#import "AppDelegate.h"
#import <Cocoa/Cocoa.h>

int main(int argc, const char * argv[]) {
    @autoreleasepool {
    }
    
    NSApplication *app = [NSApplication sharedApplication];
    AppDelegate *delegate = [AppDelegate new];
    [app setDelegate:delegate];
    
    return NSApplicationMain(argc, argv);
}
