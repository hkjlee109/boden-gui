#include <boden/widget/button.hpp>
#include <gtest/gtest.h>
#include <memory>

struct ButtonTests: public testing::Test 
{
    boden::widget::button_t mut;
    bool isButtonDown;

    void SetUp() override 
    {
        isButtonDown = false;
        return;
    }

    void TearDown() override 
    {
        return;
    }

    void GivenTheModuleIsInitialized()
    {
        mut = boden::widget::button_t();
    }

    void RegisterButtonDownHandler()
    {
        mut.add_target(this,
                       &ButtonTests::ButtonDownHander,
                       boden::widget::control_event_t::touch_down);
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
