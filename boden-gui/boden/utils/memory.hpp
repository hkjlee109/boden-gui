#pragma once

#include <memory>

namespace boden {
namespace utils {

struct shared_ptr_hash
{
    template <typename T>
    std::size_t operator()(const std::shared_ptr<T>& ptr) const noexcept
    {
        return std::hash<void*>()(ptr.get());
    }
};

struct shared_ptr_equal
{
    template <typename T>
    bool operator()(const std::shared_ptr<T>& a,
                    const std::shared_ptr<T>& b) const noexcept
    {
        return a.get() == b.get();
    }
};

} // utils
} // boden
