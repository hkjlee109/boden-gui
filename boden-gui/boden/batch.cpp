#include "batch.hpp"
#include <cstring>
#include <stdexcept>

namespace boden {

size_t batch_t::get_commands_byte_size() const
{
    return commands.size() * sizeof(boden::draw::command_t);
}

size_t batch_t::get_indices_byte_size() const
{
    return indices.size() * sizeof(boden::draw::index_t);
}

size_t batch_t::get_vertices_byte_size() const
{
    return vertices.size() * sizeof(boden::draw::vertex_t);
}

void batch_t::serialize_commands(char *buffer, size_t size) const 
{
    std::memcpy(buffer, commands.data(), size);
}

void batch_t::deserialize_commands(const char *buffer, size_t size) 
{
    const size_t count = size / sizeof(boden::draw::command_t);
    commands.resize(count);
    std::memcpy(commands.data(), buffer, count * sizeof(boden::draw::command_t));
}

void batch_t::serialize_indices(char *buffer, size_t size) const 
{
    std::memcpy(buffer, indices.data(), size);
}

void batch_t::deserialize_indices(const char *buffer, size_t size) 
{
    const size_t count = size / sizeof(boden::draw::index_t);
    indices.resize(count);
    std::memcpy(indices.data(), buffer, count * sizeof(boden::draw::index_t));
}

void batch_t::serialize_vertices(char *buffer, size_t size) const 
{
    std::memcpy(buffer, vertices.data(), size);
}

void batch_t::deserialize_vertices(const char *buffer, size_t size) 
{
    const size_t count = size / sizeof(boden::draw::vertex_t);
    vertices.resize(count);
    std::memcpy(vertices.data(), buffer, count * sizeof(boden::draw::vertex_t));
}

} // boden
