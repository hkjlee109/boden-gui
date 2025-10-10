#import "View.h"

@implementation View {
    NSTrackingArea *_trackingArea;
}

- (void)updateTrackingAreas {
    [super updateTrackingAreas];

    if (_trackingArea) {
        [self removeTrackingArea:_trackingArea];
    }

    _trackingArea = [[NSTrackingArea alloc] initWithRect:self.bounds
                                                 options:NSTrackingMouseMoved |
                                                         NSTrackingActiveAlways |
                                                         NSTrackingInVisibleRect
                                                   owner:self
                                                userInfo:nil];

    [self addTrackingArea:_trackingArea];
}

- (BOOL)acceptsFirstResponder {
    return true;
}

- (void)keyDown:(NSEvent *)event {
    if([self.viewDelegate respondsToSelector:@selector(didKeyDown:)]) {
        [self.viewDelegate didKeyDown:event];
    }
}

@end
