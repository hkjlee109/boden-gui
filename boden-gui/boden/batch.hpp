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
};

} // boden
