#include "metal_buffer_manager.hpp"

namespace platform {

metal_buffer_manager_t::metal_buffer_manager_t()
    : _last_purge_time{0}
{
}

metal_buffer_manager_t::~metal_buffer_manager_t()
{

}

metal_buffer_ref_t metal_buffer_manager_t::dequeueReusableBuffer(MTL::Device* device, uint64_t length)
{
    uint64_t now = (double)(clock_gettime_nsec_np(CLOCK_UPTIME_RAW) / 1e9);
    
    std::lock_guard<std::mutex> lock(_cache_mutex);
    
    if(now - _last_purge_time > 1)
    {
        std::vector<metal_buffer_ref_t> survivors;

        for(const metal_buffer_ref_t &buffer : _cache)
        {
            if(buffer->get_last_reuse_time() > _last_purge_time)
            {
                survivors.push_back(buffer);
            }
        }
        
        _cache = std::move(survivors);
        _last_purge_time = now;
    }
    
    std::shared_ptr<platform::metal_buffer_t> best = nullptr;
    for (const auto &candidate : _cache)
    {
        if(candidate->get_buffer()->length() >= length)
        {
            if(best == nullptr ||
               best->get_last_reuse_time() > candidate->get_last_reuse_time())
            {
                best = candidate;
            }
        }
    }

    if(best) 
    {
        _cache.erase(std::remove(_cache.begin(), _cache.end(), best), _cache.end());
        best->set_last_reuse_time(now);
        return best;
    }
    
    std::shared_ptr<platform::metal_buffer_t> buffer = std::make_shared<platform::metal_buffer_t>(device->newBuffer(length, MTL::ResourceStorageModeShared));

    return buffer;
}

void metal_buffer_manager_t::queueReusableBuffer(metal_buffer_ref_t buffer)
{
    std::lock_guard<std::mutex> lock(_cache_mutex);
    _cache.push_back(buffer);
}

} // platform
