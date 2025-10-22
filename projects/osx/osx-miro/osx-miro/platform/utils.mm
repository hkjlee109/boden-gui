#include "utils.hpp"

#include <boden/layout/point.hpp>
#include <AppKit/AppKit.h>

namespace platform {

bool utils_t::convert_to_event(void *in, boden::event_t *out)
{
    if(in == nullptr || out == nullptr)
    {
        return false;
    }
    
    NSEvent* event = (__bridge NSEvent*)(in);
    out->location = boden::layout::point_t(event.locationInWindow.x, event.locationInWindow.y);

    return true;
}

bool utils_t::convert_to_image(void *in, boden::widget::base::image_t *out)
{
    if(in == nullptr || out == nullptr)
    {
        return false;
    }
    
    NSImage* image = (__bridge NSImage*)(in);
    
    NSRect rect = NSMakeRect(0, 0, image.size.width, image.size.height);
    CGImageRef cg_image = [image CGImageForProposedRect:&rect
                                               context:nil
                                                 hints:nil];

    out->size.width = CGImageGetWidth(cg_image);
    out->size.height = CGImageGetHeight(cg_image);

    out->data.resize(out->size.width * out->size.height  * 4);
    CGContextRef context = CGBitmapContextCreate(out->data.data(),
                                                 out->size.width,
                                                 out->size.height,
                                                 8,
                                                 out->size.width * 4,
                                                 CGColorSpaceCreateDeviceRGB(),
                                                 kCGImageAlphaPremultipliedLast);
    
    if (!context)
    {
        return false;
    }

    CGContextDrawImage(context, CGRectMake(0, 0, out->size.width, out->size.height), cg_image);
    CGContextRelease(context);
    
    return true;
}

} // platform
