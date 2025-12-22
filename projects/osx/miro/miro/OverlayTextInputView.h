#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@protocol OverlayTextInputViewDelegate <NSObject>

- (void)overlayTextInputDidEndEditing:(NSNotification *)notification;

@end

@interface OverlayTextInputView : NSTextView

@property (nonatomic, weak) id<OverlayTextInputViewDelegate> overlayTextInputDelegate;

@end

NS_ASSUME_NONNULL_END
