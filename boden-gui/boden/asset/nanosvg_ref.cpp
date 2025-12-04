#include "nanosvg_ref.hpp"

#include <boden/utils/log.hpp>

#define NANOSVG_IMPLEMENTATION
#include <deps/nanosvg/nanosvg.h>

#define NANOSVGRAST_IMPLEMENTATION
#include <deps/nanosvg/nanosvgrast.h>

namespace boden {
namespace asset {

nanosvg_ref_t::nanosvg_ref_t(const char *full_path, uint32_t scale)
{    
    NSVGimage *image = nsvgParseFromFile(full_path, "px", 96);
    if(!image) 
    {
        boden::utils::log_t("nanosvg_ref_t").err("Failed to load SVG file.");
        return;
    }

    width = static_cast<int>(image->width) * scale;
    height = static_cast<int>(image->height) * scale;
    number_of_channels = 4;

    NSVGrasterizer* rasterizer = nsvgCreateRasterizer();

    _data.resize(width * height * 4);
    nsvgRasterize(rasterizer, image, 0, 0, scale, _data.data(), width, height, width * 4);

    nsvgDeleteRasterizer(rasterizer);
    nsvgDelete(image);

    uint32_t size = width * height;
    auto data = _data.data();

    for (int i = 0; i < width * height; ++i) 
    {
        float a = data[i*4+3] / 255.0f;
        data[i*4+0] = std::min(255, int(data[i * 4 + 0] * 2));
        data[i*4+1] = std::min(255, int(data[i * 4 + 1] * 2));
        data[i*4+2] = std::min(255, int(data[i * 4 + 2] * 2));
    }
}

nanosvg_ref_t::~nanosvg_ref_t()
{
}

} // asset
} // boden
