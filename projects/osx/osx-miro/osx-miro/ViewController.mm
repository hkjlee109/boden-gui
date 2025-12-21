#import "ViewController.h"

#import "RenderViewProvider.h"
#import "OverlayTextInputView.h"

#import "platform/utils.hpp"
#import <boden/event.hpp>
#import <boden/system_event.hpp>

#import <miro/cursor_type.hpp>

@interface ViewController() <NSWindowDelegate, MTKViewDelegate, ViewDelegate, OverlayTextInputViewDelegate, RenderViewProvider>

@end

@implementation ViewController {
    id<MTLDevice> _device;
    platform::osx_queue_t *_queue;

    OverlayTextInputView *_textView;
    NSCursor *_cursor;
    NSCursor *_cursorOverride;
}

- (instancetype)initWithDevice:(id<MTLDevice>)device
                         queue:(platform::osx_queue_t *)queue {
    self = [super init];
    if(self) {
        _device = device;
        _queue = queue;
        _cursor = [NSCursor openHandCursor];
        _cursorOverride = nil;
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

    _textView = [OverlayTextInputView new];
    _textView.overlayTextInputDelegate = self;
    _textView.hidden = YES;
    _textView.drawsBackground = NO;
    _textView.focusRingType = NSFocusRingTypeNone;
    _textView.backgroundColor = [NSColor clearColor];
    _textView.textColor = [NSColor labelColor];
    [self.view addSubview:_textView];
    
    View *view = (View *)self.view;
    view.cursor = _cursor;
    [view.window invalidateCursorRectsForView:view];
}

- (void)viewWillAppear {
    [super viewWillAppear];
    self.view.window.delegate = self;
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];
}

- (void)loadView {
    self.view = [[View alloc] initWithFrame:CGRectMake(0, 0, 640, 480)];
    ((View *)self.view).viewDelegate = self;
}

- (void)mouseDown:(NSEvent *)event {
    boden::event_t out_event;
    out_event.type = boden::event_type_t::left_mouse_down;
    out_event.location = boden::layout::point_t(event.locationInWindow.x,
                                                self.view.frame.size.height - event.locationInWindow.y);
    _queue->push(out_event);
}

- (void)mouseDragged:(NSEvent *)event {
    boden::event_t out_event;
    out_event.type = boden::event_type_t::left_mouse_dragged;
    out_event.location = boden::layout::point_t(event.locationInWindow.x,
                                                self.view.frame.size.height - event.locationInWindow.y);
    _queue->push(out_event);
}

- (void)mouseMoved:(NSEvent *)event {
    boden::event_t out_event;
    out_event.type = boden::event_type_t::mouse_moved;
    out_event.location = boden::layout::point_t(event.locationInWindow.x,
                                                self.view.frame.size.height - event.locationInWindow.y);
    _queue->push(out_event);
}

- (void)mouseUp:(NSEvent *)event {
    boden::event_t out_event;
    out_event.type = boden::event_type_t::left_mouse_up;
    out_event.location = boden::layout::point_t(event.locationInWindow.x,
                                                self.view.frame.size.height - event.locationInWindow.y);
    _queue->push(out_event);
}

#pragma mark MTKViewDelegate

- (void)drawInMTKView:(nonnull MTKView *)view {
    NSLog(@"# drawInMTKView");
    _queue->push(boden::event_t(boden::event_type_t::draw));
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    boden::system_event_t system_event;
    system_event.type = (uint32_t)boden::system_event_type_t::frame_change;
    system_event.params["width"] = static_cast<float>(self.displaySize.width);
    system_event.params["height"] = static_cast<float>(self.displaySize.height);
    
    boden::event_t event;
    event.type = boden::event_type_t::system;
    event.params["system_event"] = system_event;
    _queue->push(event);
}

#pragma mark NSWindowDelegate

- (void)windowWillClose:(NSNotification *)notification {
    NSLog(@"# windowWillClose");
}

#pragma mark RenderViewProvider

- (nullable id<CAMetalDrawable>)currentDrawable {
    return self.mtkView.currentDrawable;
}

- (float)displayScale {
    return self.view.window.screen.backingScaleFactor ?: NSScreen.mainScreen.backingScaleFactor;
}

- (CGSize)displaySize { 
    return self.view.bounds.size;
}

- (void)setCursor:(UInt8)type {
    switch(type)
    {
        case (uint8_t)miro::cursor_type_t::none:
            _cursor = [NSCursor arrowCursor];
            break;
            
        case (uint8_t)miro::cursor_type_t::arrow:
            _cursor = [NSCursor arrowCursor];
            break;
            
        case (uint8_t)miro::cursor_type_t::pointing_hand:
            _cursor = [NSCursor pointingHandCursor];
            break;
            
        case (uint8_t)miro::cursor_type_t::open_hand:
            _cursor = [NSCursor openHandCursor];
            break;
            
        case (uint8_t)miro::cursor_type_t::sticky:
            _cursor = [[NSCursor alloc] initWithImage:[NSImage imageNamed:@"cursor-sticky"]
                                              hotSpot:NSMakePoint(0, 0)];
            break;
            
        case (uint8_t)miro::cursor_type_t::pen:
            _cursor = [[NSCursor alloc] initWithImage:[NSImage imageNamed:@"cursor-pen"]
                                              hotSpot:NSMakePoint(0, 0)];
            break;
            
        default:
            _cursor = [NSCursor arrowCursor];
            break;
    }
    
    View *view = (View *)self.view;
    view.cursor = _cursorOverride ? _cursorOverride : _cursor;
    [view.window invalidateCursorRectsForView:view];
}

- (void)setCursorOverride:(UInt8)type {
    switch(type)
    {
        case (uint8_t)miro::cursor_type_t::none:
            _cursorOverride = nil;
            break;
            
        case (uint8_t)miro::cursor_type_t::arrow:
            _cursorOverride = [NSCursor arrowCursor];
            break;
            
        case (uint8_t)miro::cursor_type_t::pointing_hand:
            _cursorOverride = [NSCursor pointingHandCursor];
            break;
            
        case (uint8_t)miro::cursor_type_t::open_hand:
            _cursorOverride = [NSCursor openHandCursor];
            break;
            
        default:
            _cursorOverride = nil;
            break;
    }

    View *view = (View *)self.view;
    view.cursor = _cursorOverride ? _cursorOverride : _cursor;
    [view.window invalidateCursorRectsForView:view];
}

- (void)setNeedsDisplay:(BOOL)flag {
    [self.mtkView setNeedsDisplay:YES];
}

- (void)beginTextInput:(NSString *)text frame:(NSRect)frame {
    frame.origin.y = self.view.frame.size.height - frame.origin.y - frame.size.height - 7;
    _textView.string = text;
    _textView.frame = frame;
    _textView.hidden = NO;
    _textView.font = [NSFont fontWithName:@"NotoSans-Regular" size:14];

    [[self.view window] makeFirstResponder:_textView];
}

- (void)endTextInput {
    boden::system_event_t system_event;
    system_event.type = (uint32_t)boden::system_event_type_t::text_input_commit;
    system_event.params["text"] = std::string(_textView.string.UTF8String);
    
    boden::event_t event;
    event.type = boden::event_type_t::system;
    event.params["system_event"] = system_event;
    _queue->push(event);
    
    _textView.string = @"";
    _textView.hidden = YES;
    [[self.view window] makeFirstResponder:nil];
}

#pragma mark OverlayTextInputViewDelegate

- (void)overlayTextInputDidEndEditing:(nonnull NSNotification *)notification {
    OverlayTextInputView *textView = (OverlayTextInputView *)notification.object;
    NSString *text = textView.string;

    boden::system_event_t system_event;
    system_event.type = (uint32_t)boden::system_event_type_t::text_input_commit;
    system_event.params["text"] = std::string(text.UTF8String);
    
    boden::event_t event;
    event.type = boden::event_type_t::system;
    event.params["system_event"] = system_event;
    _queue->push(event);
}

#pragma mark ViewDelegate

- (void)didKeyDown:(nonnull NSEvent *)event {
    boden::event_t out_event;
    out_event.type = boden::event_type_t::key_down;
    out_event.key_code = event.keyCode;
    out_event.modifier_flags_msb = static_cast<uint32_t>(event.modifierFlags >> 32);
    out_event.modifier_flags_lsb = static_cast<uint32_t>(event.modifierFlags & 0xFFFFFFFF);
    _queue->push(out_event);
}

- (void)didKeyUp:(nonnull NSEvent *)event {
    boden::event_t out_event;
    out_event.type = boden::event_type_t::key_up;
    out_event.key_code = event.keyCode;
    out_event.modifier_flags_msb = static_cast<uint32_t>(event.modifierFlags >> 32);
    out_event.modifier_flags_lsb = static_cast<uint32_t>(event.modifierFlags & 0xFFFFFFFF);
    _queue->push(out_event);
}

- (void)didScrollWheel:(nonnull NSEvent *)event {
    boden::event_t out_event;
    out_event.type = boden::event_type_t::scroll_wheel;
    out_event.scrolling_delta_x = event.scrollingDeltaX;
    out_event.scrolling_delta_y = event.scrollingDeltaY;
    _queue->push(out_event);
}

- (void)viewDidChangeBackingProperties {
    boden::system_event_t system_event;
    system_event.type = (uint32_t)boden::system_event_type_t::backing_properties_change;
    system_event.params["scale"] = self.displayScale;
    
    boden::event_t event;
    event.type = boden::event_type_t::system;
    event.params["system_event"] = system_event;
    _queue->push(event);
}

@end
