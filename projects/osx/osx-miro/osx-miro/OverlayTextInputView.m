#import "OverlayTextInputView.h"

@implementation OverlayTextInputView

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
}

- (void)keyDown:(NSEvent *)event {
    switch(event.keyCode) {
        case 53:
            if([self.overlayTextInputDelegate respondsToSelector:@selector(overlayTextInputDidEndEditing:)]) {
                NSNotification *notification = [NSNotification notificationWithName:@"OverlayTextInputDidEndEditing"
                                                                             object:self];
                [self.overlayTextInputDelegate overlayTextInputDidEndEditing:notification];
                
                self.string = @"";
                self.hidden = YES;
            }
            return;
    }
    
    [super keyDown:event];
}

@end
