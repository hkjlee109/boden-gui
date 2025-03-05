#pragma once

#include <boden/context.hpp>
#include <vector>

namespace boden {

class view_t
{
public:
    view_t();
    virtual ~view_t();

    virtual void draw(boden::context_t &ctx) = 0;

protected:
    std::vector<std::shared_ptr<boden::view_t>> subviews;
};

} // boden
