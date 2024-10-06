#import "ViewController.h"

#import <MetalKit/MetalKit.h>
#import "Adapters/BackendAdapter.h"

#include <boden/backend.hpp>
#include <memory>

@implementation ViewController {
    MTKView* _mtkView;
    BackendAdapter* _backend;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    _mtkView = [MTKView new];
    _mtkView.device = MTLCreateSystemDefaultDevice();
    
    _mtkView.translatesAutoresizingMaskIntoConstraints = false;
    
    [self.view addSubview:_mtkView];
    
    [NSLayoutConstraint activateConstraints:@[
        [_mtkView.topAnchor constraintEqualToAnchor:self.view.topAnchor],
        [_mtkView.leadingAnchor constraintEqualToAnchor:self.view.leadingAnchor],
        [_mtkView.trailingAnchor constraintEqualToAnchor:self.view.trailingAnchor],
        [_mtkView.bottomAnchor constraintEqualToAnchor:self.view.bottomAnchor],
    ]];
    
    _backend = [[BackendAdapter alloc] initWithDrawable:_mtkView.currentDrawable
                                                 device:_mtkView.device];
}

- (void)viewDidAppear {

}

@end
