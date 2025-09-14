#pragma once

#include <boden/asset/image_info.hpp>
#include <boden/asset/texture_id.hpp>
#include <string>
#include <unordered_map>

namespace boden {
namespace asset {

class image_info_lookup_table_t
{
public:
    image_info_lookup_table_t();
    ~image_info_lookup_table_t();

    bool insert(const std::string &key, const boden::asset::image_info_t &value);
    const boden::asset::image_info_t & at(const std::string &key);

    void clear();

private:
    std::unordered_map<std::string, boden::asset::image_info_t> _table;
};

class image_info_lookup_table_ref_t
{
public:
    image_info_lookup_table_ref_t(boden::asset::image_info_lookup_table_t *instance);
    ~image_info_lookup_table_ref_t();
    
    static boden::asset::image_info_lookup_table_t * get_instance();

private:
    static std::unique_ptr<boden::asset::image_info_lookup_table_t> _instance;
};

} // asset
} // boden
