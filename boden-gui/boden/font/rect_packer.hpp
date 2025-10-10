#pragma once

#include <boden/layout/point.hpp>
#include <boden/layout/size.hpp>
#include <optional>
#include <vector>

namespace boden {
namespace font {

class rect_packer_t
{
public:
    rect_packer_t(boden::layout::size_t bin_size);
    ~rect_packer_t();

    uint8_t get_area_usage_percentage();

    std::optional<boden::layout::point_t> add_rect(boden::layout::size_t size);

    void reset(boden::layout::size_t bin_size);

private:
    struct skyline_segment_t 
    {
        float x;
        float y;
        float width;
    };

    boden::layout::size_t _bin_size;
    std::vector<skyline_segment_t> _skyline;

    float _area_usage;

    float find_rect_fits(int skylineIndex, boden::layout::size_t size) const;
    void add_skyline_segment(size_t index, skyline_segment_t &segment);
};

} // font
} // boden
