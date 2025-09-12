#pragma once

#include <boden/asset/texture_id.hpp>
#include <string>
#include <unordered_map>

namespace boden {
namespace asset {

class image_id_lookup_table_t
{
public:
    image_id_lookup_table_t();
    ~image_id_lookup_table_t();

    bool insert(const std::string &key, boden::asset::texture_id_t value);
    boden::asset::texture_id_t at(const std::string &key);

    void clear();

private:
    std::unordered_map<std::string, boden::asset::texture_id_t> _table;
};

class image_id_lookup_table_ref_t
{
public:
    image_id_lookup_table_ref_t(boden::asset::image_id_lookup_table_t *instance);
    ~image_id_lookup_table_ref_t();
    
    static boden::asset::image_id_lookup_table_t * get_instance();

private:
    static std::unique_ptr<boden::asset::image_id_lookup_table_t> _instance;
};

} // asset
} // boden
