#import "ViewController.h"

#import "platform/metal_renderer.hpp"
#import <app/main_view.hpp>

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
    self.mtkView.delegate = self;
    
    _main_view = std::make_unique<app::main_view_t>();
    _renderer = std::make_unique<platform::metal_renderer_t>((__bridge CA::MetalDrawable*)self.mtkView.currentDrawable,
                                                             (__bridge MTL::Device *)self.mtkView.device);
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
    _main_view->draw(_renderer.get());
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    NSLog(@"# mtkView drawableSizeWillChange");
}

#pragma mark NSWindowDelegate

- (void)windowWillClose:(NSNotification *)notification {
    NSLog(@"# windowWillClose");
}

@end
