#import "ViewController.h"

#import <MetalKit/MetalKit.h>

@interface ViewController() <NSWindowDelegate, MTKViewDelegate>

@end

@implementation ViewController

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
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    NSLog(@"# mtkView drawableSizeWillChange");
}

#pragma mark NSWindowDelegate

- (void)windowWillClose:(NSNotification *)notification {
    NSLog(@"# windowWillClose");
}

@end
