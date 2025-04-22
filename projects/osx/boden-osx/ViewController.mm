#import "ViewController.h"

#import "platform/mtl_image_library.hpp"
#import "platform/mtl_renderer.hpp"
#import "platform/utils.hpp"
#import <app/main_view_controller.hpp>
#import <boden/asset/image_library_ref.hpp>
#import <boden/context.hpp>
#import <boden/event.hpp>
#import <boden/image.hpp>

@interface ViewController() <NSWindowDelegate, MTKViewDelegate>

@end

@implementation ViewController {
    std::unique_ptr<app::main_view_controller_t> _main_view_controller;
    std::unique_ptr<platform::mtl_renderer_t> _renderer;
    std::unique_ptr<boden::asset::image_library_ref_t> _image_library;
}

- (MTKView*)mtkView {
    return (MTKView *)self.view;
}

- (CAMetalLayer*)mtkLayer {
    return (CAMetalLayer *)self.mtkView.layer;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.mtkView.device = MTLCreateSystemDefaultDevice();
    self.mtkView.enableSetNeedsDisplay = YES;
    self.mtkView.delegate = self;

    platform::mtl_image_library_t *mtl_image_library{new platform::mtl_image_library_t((__bridge MTL::Device *)self.mtkView.device)};
    
    NSImage* nsimage = [NSImage imageNamed:@"gearshape"];
    boden::image_t image;
    platform::utils_t::convert_to_image((__bridge void *)nsimage, &image);
    mtl_image_library->load_image_from_data("gearshape", image);
    
    _image_library = std::make_unique<boden::asset::image_library_ref_t>(mtl_image_library);
    _main_view_controller = std::make_unique<app::main_view_controller_t>(boden::layout::rect_t{0, 0, 640, 480});
    _renderer = std::make_unique<platform::mtl_renderer_t>((__bridge MTL::Device *)self.mtkView.device, mtl_image_library);
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
    
    out_event.location.y = self.view.frame.size.height - out_event.location.y;

    std::shared_ptr<boden::view_t> target = _main_view_controller->get_view().hit_test(out_event.location);
    if(target == nullptr) return;

    target->mouse_down(out_event);
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

    _main_view_controller->draw(ctx);
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
    NSLog(@"# mtkView drawableSizeWillChange");
}

#pragma mark NSWindowDelegate

- (void)windowWillClose:(NSNotification *)notification {
    NSLog(@"# windowWillClose");
}

@end
