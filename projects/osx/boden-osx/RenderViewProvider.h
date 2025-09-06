#import <Foundation/Foundation.h>
#import <MetalKit/MetalKit.h>

@protocol RenderViewProvider <NSObject>

@required
- (nullable id<CAMetalDrawable>)currentDrawable;
- (CGSize)displaySize;
- (float)displayScale;

@end
