#import "ViewController.h"

#import "platform/metal_renderer.hpp"
#import <app/main_view.hpp>
#import <boden/context.hpp>

@interface ViewController() <NSWindowDelegate, MTKViewDelegate>

@end

@implementation ViewController {
    std::unique_ptr<app::main_view_t> _main_view;
    std::unique_ptr<platform::metal_renderer_t> _renderer;
}

- (MTKView*)mtkView {
    return (MTKView*)self.view;
}

- (CAMetalLayer*)metalLayer {
    return (CAMetalLayer *)self.mtkView.layer;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.mtkView.device = MTLCreateSystemDefaultDevice();
    self.mtkView.enableSetNeedsDisplay = YES;
    self.mtkView.delegate = self;

    _main_view = std::make_unique<app::main_view_t>();
    _renderer = std::make_unique<platform::metal_renderer_t>((__bridge MTL::Device *)self.mtkView.device);
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

#pragma mark MTKViewDelegate

- (void)drawInMTKView:(nonnull MTKView *)view {
    NSLog(@"# drawInMTKView");
    boden::context_t ctx;
    ctx.renderer = _renderer.get();
    ctx.surface_handle = (boden::surface_handle_t)(__bridge CA::MetalDrawable *)self.mtkView.currentDrawable;
    
    ctx.display_size = boden::layout::size_t{(float)(self.view.bounds.size.width),
                                               (float)(self.view.bounds.size.height)};

    CGFloat scale = self.view.window.screen.backingScaleFactor ?: NSScreen.mainScreen.backingScaleFactor;
    ctx.display_scale = boden::layout::vec2_t{(float)scale, (float)scale};

    _main_view->draw(ctx);
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    NSLog(@"# mtkView drawableSizeWillChange");
}

#pragma mark NSWindowDelegate

- (void)windowWillClose:(NSNotification *)notification {
    NSLog(@"# windowWillClose");
}

@end
