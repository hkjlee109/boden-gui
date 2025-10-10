#include "rect_packer.hpp"

#include <algorithm>
#include <boden/layout/rect.hpp>
#include <iostream>

namespace boden {
namespace font {

rect_packer_t::rect_packer_t(boden::layout::size_t bin_size)
{
    reset(bin_size);
}

rect_packer_t::~rect_packer_t()
{
}

uint8_t rect_packer_t::get_area_usage_percentage() 
{
    return static_cast<uint8_t>(_area_usage / (_bin_size.height * _bin_size.width) * 100);
}

std::optional<boden::layout::point_t> rect_packer_t::add_rect(boden::layout::size_t size_)
{
    boden::layout::size_t size{size_.width + 1, size_.height + 1};
    
    if(size.width > _bin_size.width || size.height > _bin_size.height) 
    {
        return std::nullopt;
    }

    skyline_segment_t new_segment{0, 
                                  std::numeric_limits<float>::max(), 
                                  std::numeric_limits<float>::max()};

    size_t new_segment_index = -1;

    for(size_t i = 0; i < _skyline.size(); ++i) 
    {
        float y = find_rect_fits(i, size);
        if(y == -1)
        {
            continue;
        }

        if (y < new_segment.y || 
            (y == new_segment.y && _skyline[i].width < new_segment.width)) 
        {
            new_segment.x = _skyline[i].x;
            new_segment.y = y;
            new_segment.width = _skyline[i].width;
            new_segment_index = i;
        }
    }

    if(new_segment_index == -1)
    {
        return std::nullopt;
    }

    boden::layout::point_t origin{new_segment.x, new_segment.y};

    new_segment.y = new_segment.y + size.height;
    new_segment.width = size.width;
    _area_usage = size.width * size.height;

    add_skyline_segment(new_segment_index, new_segment);

    return origin;
}

void rect_packer_t::reset(boden::layout::size_t bin_size)
{
    _bin_size = bin_size;
    _area_usage = 0;
    _skyline.clear();
    _skyline.emplace_back(0, 0, _bin_size.width);
}

float rect_packer_t::find_rect_fits(int segment_index, boden::layout::size_t size) const 
{
    float x = _skyline[segment_index].x;
    if(x + size.width > _bin_size.width)
    {
        return -1;
    }

    float width_remaining = size.width;
    float y = _skyline[segment_index].y;

    size_t i = segment_index;

    while(width_remaining > 0) 
    {
        y = std::max(y, _skyline[i].y);
        if(y + size.height > _bin_size.height)
        {
            return -1;
        }

        width_remaining -= _skyline[i++].width;

        if(i >= _skyline.size() && width_remaining > 0)
        {
            return -1;
        }
    }

    return y;
}

void rect_packer_t::add_skyline_segment(size_t index, skyline_segment_t &segment)
{
    _skyline.insert(_skyline.begin() + index, segment);

    for(size_t i = index + 1; i < _skyline.size(); ++i) 
    {
        float old_right = _skyline[i - 1].x + _skyline[i - 1].width;
        if(_skyline[i].x < old_right) 
        {
            float dw = old_right - _skyline[i].x;
            _skyline[i].x += dw;
            _skyline[i].width -= dw;

            if(_skyline[i].width <= 0) 
            {
                _skyline.erase(_skyline.begin() + i);
                --i;
            } 
            else 
            {
                break;
            }
        } 
        else 
        {
            break;
        }
    }

    for(size_t i = 0; i < _skyline.size() - 1; ++i) 
    {
        if(_skyline[i].y == _skyline[i + 1].y) 
        {
            _skyline[i].width += _skyline[i + 1].width;
            _skyline.erase(_skyline.begin() + i + 1);
            --i;
        }
    }
}

} // font
} // boden
