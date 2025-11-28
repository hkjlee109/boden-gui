#import <MetalKit/MetalKit.h>

NS_ASSUME_NONNULL_BEGIN


@protocol ViewDelegate <NSObject>

- (void)didKeyDown:(NSEvent *)event;
- (void)didKeyUp:(NSEvent *)event;

- (void)viewDidChangeBackingProperties;

@end

@interface View : MTKView

@property (nonatomic, weak) id<ViewDelegate> viewDelegate;

@end

NS_ASSUME_NONNULL_END
