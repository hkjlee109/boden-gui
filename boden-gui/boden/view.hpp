#pragma once

#include <boden/context.hpp>

namespace boden {

class view_t
{
public:
    view_t();
    virtual ~view_t();

    virtual void draw(boden::context_t &ctx) = 0;

private:
};

} // boden
