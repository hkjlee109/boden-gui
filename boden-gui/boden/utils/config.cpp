#include "config.hpp"

#include <boost/json.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

namespace boden {
namespace utils {

std::vector<config_t::image_entry_t> config_t::parse_images_config_file(const std::string &filename)
{
    std::vector<image_entry_t> r;

    std::ifstream file{filename};
    if (!file)
    {
        std::cerr << "Error opening the JSON file. " << filename << std::endl;
        return r;
    }

    std::string raw_json = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    try 
    {
        boost::json::value parsed = boost::json::parse(raw_json);
        boost::json::object root = parsed.as_object();

        if(!root.contains("images")) 
        {
            return r;
        }

        boost::json::object images = root["images"].as_object();

        for(const auto &[id, entry_value] : images) 
        {
            const boost::json::object& entry = entry_value.as_object();
            r.emplace_back(config_t::image_entry_t{std::string(id), 
                                                   std::string(entry.at("path").as_string().c_str()),
                                                   std::string(entry.at("name").as_string().c_str()),
                                                   std::string(entry.at("type").as_string().c_str())});
        }
    } 
    catch(const std::exception &e) 
    {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }

    return r;
};

std::vector<config_t::image_entry_t> config_t::parse_fonts_config_file(const std::string &filename)
{
    std::vector<image_entry_t> r;

    std::ifstream file{filename};
    if (!file)
    {
        std::cerr << "Error opening the JSON file. " << filename << std::endl;
        return r;
    }

    std::string raw_json = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    try 
    {
        boost::json::value parsed = boost::json::parse(raw_json);
        boost::json::object root = parsed.as_object();

        if(!root.contains("fonts")) 
        {
            return r;
        }

        boost::json::object images = root["fonts"].as_object();

        for(const auto &[id, entry_value] : images) 
        {
            const boost::json::object& entry = entry_value.as_object();
            r.emplace_back(config_t::image_entry_t{std::string(id), 
                                                   std::string(entry.at("path").as_string().c_str()),
                                                   std::string(entry.at("name").as_string().c_str()),
                                                   std::string(entry.at("type").as_string().c_str())});
        }
    } 
    catch(const std::exception &e) 
    {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }

    return r;
};

} // utils
} // boden
