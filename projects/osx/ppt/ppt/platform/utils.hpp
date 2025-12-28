#pragma once

#include <boden/event.hpp>
#include <boden/widget/base/image.hpp>

namespace platform {

class utils_t
{
public:
    static bool convert_to_event(void *in, boden::event_t *out);
    static bool convert_to_image(void *in, boden::widget::base::image_t *out);
};

} // platform
