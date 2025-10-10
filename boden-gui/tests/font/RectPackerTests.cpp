#include <boden/font/rect_packer.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <optional>

struct RectPackerTests: public testing::Test
{
    boden::font::rect_packer_t mut;
    std::optional<boden::layout::point_t> addResult;

    RectPackerTests()
        : mut({0, 0})
    {
    }

    void SetUp() override 
    {
        addResult = std::nullopt;
        return;
    }

    void TearDown() override 
    {
        return;
    }

    void GivenTheModuleIsInitialized(float width, float height)
    {
        mut = boden::font::rect_packer_t({width, height});
    }

    void AddRectangle(float width, float height)
    {
        addResult = mut.add_rect({width, height});
    }

    void ShouldFailAddRectangle()
    {
        ASSERT_FALSE(addResult.has_value());
    }

    void ShouldAddRectangleAt(float x, float y)
    {
        ASSERT_TRUE(addResult.has_value());
        EXPECT_EQ(addResult->x, x);
        EXPECT_EQ(addResult->y, y);
    }
};

TEST_F(RectPackerTests, ShouldAdd) 
{
    GivenTheModuleIsInitialized(5, 5);

    AddRectangle(5, 5);
    ShouldAddRectangleAt(0, 0);
}

TEST_F(RectPackerTests, ShouldNotAddWhenRunningOutOfSpaces) 
{
    GivenTheModuleIsInitialized(5, 5);

    AddRectangle(6, 5);
    ShouldFailAddRectangle();

    AddRectangle(5, 5);
    AddRectangle(1, 1);
    ShouldFailAddRectangle();
}

TEST_F(RectPackerTests, ShouldAddMultipleRectangles) 
{
    GivenTheModuleIsInitialized(5, 2);

    AddRectangle(3, 1);
    ShouldAddRectangleAt(0, 0);

    AddRectangle(2, 1);
    ShouldAddRectangleAt(3, 0);

    AddRectangle(2, 1);
    ShouldAddRectangleAt(0, 1);

    AddRectangle(3, 1);
    ShouldAddRectangleAt(2, 1);

    AddRectangle(1, 1);
    ShouldFailAddRectangle();
}

TEST_F(RectPackerTests, ShouldAddMultipleRectanglesV2) 
{
    GivenTheModuleIsInitialized(5, 2);

    AddRectangle(4, 1);
    ShouldAddRectangleAt(0, 0);

    AddRectangle(4, 1);
    ShouldAddRectangleAt(0, 1);

    AddRectangle(1, 2);
    ShouldAddRectangleAt(4, 0);

    AddRectangle(1, 1);
    ShouldFailAddRectangle();
}

TEST_F(RectPackerTests, ShouldAddMultipleRectanglesV3) 
{
    GivenTheModuleIsInitialized(5, 2);

    AddRectangle(1, 2);
    ShouldAddRectangleAt(0, 0);

    AddRectangle(3, 1);
    ShouldAddRectangleAt(1, 0);

    AddRectangle(1, 2);
    ShouldAddRectangleAt(4, 0);

    AddRectangle(3, 1);
    ShouldAddRectangleAt(1, 1);

    AddRectangle(1, 1);
    ShouldFailAddRectangle();
}

TEST_F(RectPackerTests, ShouldAddMultipleRectanglesV4) 
{
    GivenTheModuleIsInitialized(5, 3);

    AddRectangle(1, 1);
    ShouldAddRectangleAt(0, 0);

    AddRectangle(3, 2);
    ShouldAddRectangleAt(1, 0);

    AddRectangle(3, 1);
    ShouldAddRectangleAt(0, 2);

    AddRectangle(1, 3);
    ShouldAddRectangleAt(4, 0);

    AddRectangle(1, 1);
    ShouldAddRectangleAt(3, 2);

    AddRectangle(1, 1);
    ShouldFailAddRectangle();

    // 1x1 block at (0,1) is wasted.
}