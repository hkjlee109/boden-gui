#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace boden {
namespace utils {

class config_t
{
public:
    struct image_entry_t
    {
        std::string key;
        std::string path;
        std::string name;
        std::string type; 
        uint32_t scale;
    };

    static std::vector<config_t::image_entry_t> parse_images_config_file(const std::string &filename);
    static std::vector<config_t::image_entry_t> parse_fonts_config_file(const std::string &filename);
};

} // utils
} // boden
