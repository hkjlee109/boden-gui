#import "ViewController.h"
#import "RenderViewProvider.h"

#import "platform/utils.hpp"
#import <boden/event.hpp>

@interface ViewController() <NSWindowDelegate, MTKViewDelegate, RenderViewProvider>

@end

@implementation ViewController {
    id<MTLDevice> _device;
    platform::osx_queue_t *_queue;
}

- (instancetype)initWithDevice:(id<MTLDevice>)device
                         queue:(platform::osx_queue_t *)queue {
    self = [super init];
    if(self) {
        _device = device;
        _queue = queue;
    }
    return self;
}

- (MTKView *)mtkView {
    return (MTKView *)self.view;
}

- (CAMetalLayer *)mtkLayer {
    return (CAMetalLayer *)self.mtkView.layer;
}

- (void)viewDidLoad {
    [super viewDidLoad];

    self.mtkView.device = _device;
    self.mtkView.enableSetNeedsDisplay = YES;
    self.mtkView.delegate = self;
}

- (void)viewWillAppear {
    [super viewWillAppear];
    self.view.window.delegate = self;
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];
}

- (void)loadView {
    self.view = [[MTKView alloc] initWithFrame:CGRectMake(0, 0, 640, 480)];
}

- (void)mouseDown:(NSEvent *)event {
    boden::event_t out_event;
    platform::utils_t::convert_to_event((__bridge void *)event, &out_event);
    
    out_event.type = boden::event_type_t::left_mouse_down;
    out_event.location.y = self.view.frame.size.height - out_event.location.y;
    _queue->push(out_event);
}

- (void)mouseDragged:(NSEvent *)event {
    boden::event_t out_event;
    platform::utils_t::convert_to_event((__bridge void *)event, &out_event);
    
    out_event.type = boden::event_type_t::left_mouse_dragged;
    out_event.location.y = self.view.frame.size.height - out_event.location.y;
    _queue->push(out_event);
}

- (void)mouseMoved:(NSEvent *)event {
    boden::event_t out_event;
    platform::utils_t::convert_to_event((__bridge void *)event, &out_event);
    
    out_event.type = boden::event_type_t::mouse_moved;
    out_event.location.y = self.view.frame.size.height - out_event.location.y;
    _queue->push(out_event);
}

- (void)mouseUp:(NSEvent *)event {
    boden::event_t out_event;
    platform::utils_t::convert_to_event((__bridge void *)event, &out_event);
    
    out_event.type = boden::event_type_t::left_mouse_up;
    out_event.location.y = self.view.frame.size.height - out_event.location.y;
    _queue->push(out_event);
}

#pragma mark MTKViewDelegate

- (void)drawInMTKView:(nonnull MTKView *)view {
    NSLog(@"# drawInMTKView");
    _queue->push(boden::event_t(boden::event_type_t::draw));
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    NSLog(@"# mtkView drawableSizeWillChange");
}

#pragma mark NSWindowDelegate

- (void)windowWillClose:(NSNotification *)notification {
    NSLog(@"# windowWillClose");
}

#pragma mart RenderViewProvider
- (nullable id<CAMetalDrawable>)currentDrawable {
    return self.mtkView.currentDrawable;
}

- (float)displayScale {
    return self.view.window.screen.backingScaleFactor ?: NSScreen.mainScreen.backingScaleFactor;
}

- (CGSize)displaySize { 
    return self.view.bounds.size;
}

- (void)setNeedsDisplay:(BOOL)flag {
    [self.mtkView setNeedsDisplay:YES];
}

@end
