#pragma once

#include <memory>

namespace miro {
namespace utils {

struct weak_ptr_hash 
{
    template <typename T>
    std::size_t operator()(const std::weak_ptr<T>& wp) const 
    {
        auto sp = wp.lock();
        return std::hash<T*>()(sp.get());
    }
};

struct weak_ptr_equal 
{
    template <typename T>
    bool operator()(const std::weak_ptr<T>& a, const std::weak_ptr<T>& b) const 
    {
        return a.lock().get() == b.lock().get();
    }
};

} // utils
} // miro
