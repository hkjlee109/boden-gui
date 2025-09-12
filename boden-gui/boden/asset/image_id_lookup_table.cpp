#include "image_id_lookup_table.hpp"

namespace boden {
namespace asset {

image_id_lookup_table_t::image_id_lookup_table_t()
{
}

image_id_lookup_table_t::~image_id_lookup_table_t()
{
}

bool image_id_lookup_table_t::insert(const std::string &key, boden::asset::texture_id_t value)
{
    _table.insert({key, value});
    return true;
}

boden::asset::texture_id_t image_id_lookup_table_t::at(const std::string &key)
{
    try 
    {
        return _table.at(key);
    } 
    catch(const std::out_of_range &e) 
    {
        throw std::runtime_error("Image name(key) not found: " + key);
    }
}

void image_id_lookup_table_t::clear()
{
    _table.clear();
}

std::unique_ptr<boden::asset::image_id_lookup_table_t> image_id_lookup_table_ref_t::_instance = nullptr;

image_id_lookup_table_ref_t::image_id_lookup_table_ref_t(boden::asset::image_id_lookup_table_t *instance)
{
    _instance.reset(instance);
}

image_id_lookup_table_ref_t::~image_id_lookup_table_ref_t()
{
    if(_instance) 
    {
        _instance.reset();
    }
}

boden::asset::image_id_lookup_table_t * image_id_lookup_table_ref_t::get_instance()
{
    return _instance.get();
}

} // asset
} // boden
