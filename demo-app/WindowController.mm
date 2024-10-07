#import "WindowController.h"

#import "AppViewController.h"

@interface WindowController ()

@end

@implementation WindowController

- (id)init {
    self = [super init];
    if(self) {
        NSPanel* panel = [
            [NSPanel alloc]
            initWithContentRect:NSZeroRect
            styleMask: NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable
            backing:NSBackingStoreBuffered
            defer:YES
        ];
        
        panel.floatingPanel = YES;
        panel.titlebarAppearsTransparent = YES;
        [panel setFrame:NSMakeRect(0, 0, 1200, 720) display:YES animate:NO];
        
        AppViewController* controller = [AppViewController new];

        [panel setContentViewController:controller];
        [panel setRestorable:NO];
        [panel center];
        
        self.window = panel;
    }
    return self;
}

@end
