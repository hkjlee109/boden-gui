#pragma once

#include <boden/draw/command.hpp>
#include <boden/draw/index.hpp>
#include <boden/draw/vertex.hpp>
#include <vector>

namespace boden {

struct batch_t
{
    std::vector<boden::draw::command_t> commands;
    std::vector<boden::draw::index_t> indices;
    std::vector<boden::draw::vertex_t> vertices;

    size_t get_commands_byte_size() const;
    size_t get_indices_byte_size() const;
    size_t get_vertices_byte_size() const;

    void serialize_commands(char *buffer, size_t size) const;
    void deserialize_commands(const char *buffer, size_t size);

    void serialize_indices(char *buffer, size_t size) const;
    void deserialize_indices(const char *buffer, size_t size);

    void serialize_vertices(char *buffer, size_t size) const;
    void deserialize_vertices(const char *buffer, size_t size);
};

} // boden
