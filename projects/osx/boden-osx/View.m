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

@end
