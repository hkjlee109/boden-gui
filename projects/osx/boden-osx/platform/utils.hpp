#pragma once

#include <boden/event.hpp>

namespace platform {

class utils_t
{
public:
    static bool convert_to_event(void *in, boden::event_t *out);
};

} // platform
