#include <boden/widget/view.hpp>
#include <gtest/gtest.h>
#include <memory>

struct ViewTests: public testing::Test 
{
    std::shared_ptr<boden::widget::view_t> mut;
    std::shared_ptr<boden::widget::view_t> another_view;

    void SetUp() override 
    {
        return;
    }

    void TearDown() override 
    {
        mut.reset();
        another_view.reset();
        return;
    }

    void GivenTheModuleIsInitialized(const boden::layout::rect_t &frame)
    {
        mut = std::make_shared<boden::widget::view_t>(frame);
    }

    void GivenAnotherViewIsInitializedAndAdded(const boden::layout::rect_t &frame)
    {
        another_view = std::make_shared<boden::widget::view_t>(frame);
        mut->add_subview(another_view);
    }

    void HideView(std::shared_ptr<boden::widget::view_t> &view)
    {
        view->set_hidden(true);
    }

    void HitTestShouldReturn(const boden::layout::point_t &point, boden::widget::view_t *expected)
    {
        std::shared_ptr<boden::widget::view_t> actual = mut->hit_test(point);
        EXPECT_EQ(
            expected,
            actual.get()
        );
    }
};

TEST_F(ViewTests, ShouldHitTestReturnFarthestView) 
{
    GivenTheModuleIsInitialized({0, 0, 100, 100});
    GivenAnotherViewIsInitializedAndAdded({40, 40, 10, 10});

    HitTestShouldReturn({10, 10}, mut.get());
    HitTestShouldReturn({45, 45}, another_view.get());
}

TEST_F(ViewTests, ShouldHitTestBeIgnoredWhenHidden) 
{
    GivenTheModuleIsInitialized({0, 0, 100, 100});
    GivenAnotherViewIsInitializedAndAdded({40, 40, 10, 10});

    HideView(another_view);

    HitTestShouldReturn({45, 45}, mut.get());
}
