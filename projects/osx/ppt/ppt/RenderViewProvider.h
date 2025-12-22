#import <Foundation/Foundation.h>
#import <MetalKit/MetalKit.h>

@protocol RenderViewProvider <NSObject>

- (nullable id<CAMetalDrawable>)currentDrawable;
- (CGSize)displaySize;
- (float)displayScale;

- (void)setNeedsDisplay:(BOOL)flag;
- (void)beginTextInput:(NSString *_Nonnull)text frame:(NSRect)frame;
- (void)endTextInput;

@end
