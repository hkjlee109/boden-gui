#include <boden/batch.hpp>
#include <gtest/gtest.h>

struct BatchTests: public testing::Test 
{
    boden::batch_t mut;
    std::vector<char> serialized_commands;
    std::vector<char> serialized_indices;
    std::vector<char> serialized_vertices;

    boden::batch_t deserialized_batch;

    void SetUp() override 
    {
        return;
    }

    void TearDown() override 
    {
        return;
    }

    void GivenTheModuleIsInitialized()
    {
        mut = boden::batch_t();
    }

    void GivenTheModuleIsPopulated()
    {
        mut.commands.push_back({1, 2, 3, {4, 5, 6, 7}});
        mut.commands.push_back({7, 6, 5, {4, 3, 2, 1}, 10});
        mut.indices.push_back(99);
        mut.indices.push_back(88);
        mut.vertices.push_back({{1.0f, 2.0f}, {0.5f, 0.5f}, 0xFF0000FF});
        mut.vertices.push_back({{3.0f, 4.0f}, {1.0f, 1.0f}, 0x00FF00FF});
        mut.vertices.push_back({{1.0f, 2.0f}, {0.5f, 1.0f}, 0xFF00FFFF});
    }
    
    void Serialize()
    {
        const size_t commands_size = mut.get_commands_byte_size();
        serialized_commands.resize(commands_size);
        mut.serialize_commands(serialized_commands.data(), commands_size);

        const size_t indices_size = mut.get_indices_byte_size();
        serialized_indices.resize(indices_size);
        mut.serialize_indices(serialized_indices.data(), indices_size);
    
        const size_t vertices_size = mut.get_vertices_byte_size();
        serialized_vertices.resize(vertices_size);
        mut.serialize_vertices(serialized_vertices.data(), vertices_size);
    }

    void Deserialize()
    {
        deserialized_batch = boden::batch_t();
        deserialized_batch.deserialize_commands(serialized_commands.data(), 
                                                serialized_commands.size());
        deserialized_batch.deserialize_indices(serialized_indices.data(), 
                                               serialized_indices.size());                                                
        deserialized_batch.deserialize_vertices(serialized_vertices.data(), 
                                                serialized_vertices.size());
    }

    void DeserializedBatchShouldEqualOriginal()
    {
        ASSERT_EQ(mut.vertices.size(), deserialized_batch.vertices.size());

        for(size_t i = 0; i < mut.commands.size(); ++i) 
        {
            const auto &a = mut.commands[i];
            const auto &b = deserialized_batch.commands[i];

            EXPECT_EQ(a.count, b.count);
            EXPECT_EQ(a.index_buffer_offset, b.index_buffer_offset);
            EXPECT_EQ(a.vertex_buffer_offset, b.vertex_buffer_offset);
            EXPECT_FLOAT_EQ(a.clip_rect.origin.x, b.clip_rect.origin.x);
            EXPECT_FLOAT_EQ(a.clip_rect.origin.y, b.clip_rect.origin.y);
            EXPECT_FLOAT_EQ(a.clip_rect.size.width, b.clip_rect.size.width);
            EXPECT_FLOAT_EQ(a.clip_rect.size.height, b.clip_rect.size.height);
            EXPECT_EQ(a.texture_id, b.texture_id);
        }

        for (size_t i = 0; i < mut.indices.size(); ++i) 
        {
            EXPECT_EQ(mut.indices[i], deserialized_batch.indices[i]);
        }

        for(size_t i = 0; i < mut.vertices.size(); ++i) 
        {
            const auto &a = mut.vertices[i];
            const auto &b = deserialized_batch.vertices[i];

            EXPECT_FLOAT_EQ(a.position.x, b.position.x);
            EXPECT_FLOAT_EQ(a.position.y, b.position.y);
            EXPECT_FLOAT_EQ(a.uv.x, b.uv.x);
            EXPECT_FLOAT_EQ(a.uv.y, b.uv.y);
            EXPECT_EQ(a.color, b.color);
        }
    }
};

TEST_F(BatchTests, ShouldSerialize) 
{
    GivenTheModuleIsInitialized();
    GivenTheModuleIsPopulated();

    Serialize();
    Deserialize();

    DeserializedBatchShouldEqualOriginal();
}
