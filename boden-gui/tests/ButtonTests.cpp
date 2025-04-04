#include <boden/button.hpp>
#include <gtest/gtest.h>
#include <memory>

struct ButtonTests: public testing::Test 
{
    boden::button_t mut;
    bool isButtonDown;

    virtual void SetUp() override 
    {
        isButtonDown = false;
        return;
    }

    virtual void TearDown() override 
    {
        return;
    }

    void GivenTheModuleIsInitialized()
    {
        mut = boden::button_t();
    }

    void RegisterButtonDownHandler()
    {
        mut.add_target(this,
                       &ButtonTests::ButtonDownHander,
                       boden::control_event_t::touch_down);
    }

    void ButtonDownHander() 
    {
        isButtonDown = true;
    }

    void ButtonDownOccured() 
    {
        mut.mouse_down(boden::event_t{});
    }

    void ButtonDownHandlerShouldBeCalled() 
    {
        EXPECT_EQ(true, isButtonDown);
    }
};

TEST_F(ButtonTests, ShouldEventBeIgnoredWhenDisabled) 
{
    GivenTheModuleIsInitialized();
    RegisterButtonDownHandler();

    ButtonDownOccured();

    ButtonDownHandlerShouldBeCalled();
}
