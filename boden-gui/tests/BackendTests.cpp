#include <boden/backend.hpp>
#include <gtest/gtest.h>

struct BackendTests: public testing::Test 
{
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
    }
};

TEST_F(BackendTests, FirstTest) 
{
    GivenTheModuleIsInitialized();
}
