#pragma once

#include <memory>
#include <mutex>
#include <vector>
#include <Metal/Metal.hpp>

namespace platform {

struct metal_buffer_t;
using metal_buffer_ref_t = std::shared_ptr<platform::metal_buffer_t>;

struct metal_buffer_t
{
    metal_buffer_t(MTL::Buffer *buffer)
        : _buffer{buffer},
          _last_reuse_time{0}
    {
    }
    
    ~metal_buffer_t()
    {
        if(_buffer)
        {
            _buffer->release();
        }
    }
    
    MTL::Buffer * get_buffer() const
    {
        return _buffer;
    }
    
    uint64_t get_last_reuse_time() const
    {
        return _last_reuse_time;
    }
    
    void set_last_reuse_time(uint64_t time)
    {
        _last_reuse_time = time;
    }

private:
    MTL::Buffer *_buffer;
    uint64_t _last_reuse_time;
};

class metal_buffer_manager_t
{
public:
    metal_buffer_manager_t();
    ~metal_buffer_manager_t();
    
    metal_buffer_ref_t dequeueReusableBuffer(MTL::Device* device, uint64_t length);
    void queueReusableBuffer(metal_buffer_ref_t buffer);

private:
    std::vector<metal_buffer_ref_t> _cache;
    std::mutex _cache_mutex;
    
    uint64_t _last_purge_time;
};

} // platform
