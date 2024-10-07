#import "AppViewController.h"

#import <MetalKit/MetalKit.h>
#import "Adapters/BackendAdapter.h"

@interface AppViewController () <NSWindowDelegate, MTKViewDelegate>

@end

@implementation AppViewController {
    BackendAdapter* _backend;
}

-(MTKView *)mtkView {
    return (MTKView*)self.view;
}

-(void)loadView {
    self.view = [[MTKView alloc] initWithFrame:CGRectMake(0, 0, 1200, 720)];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.mtkView.device = MTLCreateSystemDefaultDevice();
    self.mtkView.delegate = self;
    
    _backend = [[BackendAdapter alloc]
        initWithDrawable:self.mtkView.currentDrawable
                  device:self.mtkView.device
    ];
    
    [_backend draw];
}

- (void)viewWillAppear {
    [super viewWillAppear];
    self.view.window.delegate = self;
}

#pragma mark MTKViewDelegate
- (void)drawInMTKView:(nonnull MTKView *)view {
    NSLog(@"# drawInMTKView");
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size { 
    NSLog(@"# mtkView");
}

#pragma mark NSWindowDelegate
- (void)windowWillClose:(NSNotification *)notification {
    NSLog(@"# windowWillClose");
}

@end
