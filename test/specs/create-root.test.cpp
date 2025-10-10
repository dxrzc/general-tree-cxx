#include "doctest.h"
#include "general-tree.h"
#include "utils/fixtures/lifecycle-counter.fixture.h"

TEST_CASE_FIXTURE(LifecycleCounterFixture, "create_root")
{
    SUBCASE("throw runtime error if root already exists")
    {
        general_tree<int> empty_tree (1);        
        CHECK_THROWS_AS(empty_tree.create_root(2), std::runtime_error);
    }

    SUBCASE("create one copy of the provided value and store it in root")
    {
        general_tree<LifecycleCounter> gt;
        LifecycleCounter value("string11", 11);
        gt.create_root(value);

        CHECK_EQ(gt.root().data().get_int(), value.get_int());
        CHECK_EQ(gt.root().data().get_string(), value.get_string());
        CHECK_EQ(LifecycleCounter::move_constructor_calls, 0);
        CHECK_EQ(LifecycleCounter::copy_constructor_calls, 1);
    }
}

TEST_CASE_FIXTURE(LifecycleCounterFixture, "create_root (move)")
{
    SUBCASE("provided value is stored in root with no copies")
    {
        LifecycleCounter value("string001", 1);
        general_tree<LifecycleCounter> gt;
        gt.create_root(std::move(value));

        CHECK_EQ(gt.root().data().get_int(), 1);
        CHECK_EQ(gt.root().data().get_string(), "string001");
        CHECK_EQ(LifecycleCounter::copy_constructor_calls, 0);
        CHECK_EQ(LifecycleCounter::move_constructor_calls, 1);
    }

    SUBCASE("temporary value triggers move")
    {
        general_tree<LifecycleCounter> gt;
        gt.create_root(LifecycleCounter{});
        CHECK_EQ(LifecycleCounter::copy_constructor_calls, 0);
        CHECK_EQ(LifecycleCounter::move_constructor_calls, 1);
    }
}

TEST_CASE_FIXTURE(LifecycleCounterFixture, "emplace_root")
{
    SUBCASE("throw runtime error if root already exists")
    {
        general_tree<int> empty_tree;
        empty_tree.emplace_root(1);
        CHECK_THROWS_AS(empty_tree.create_root(2), std::runtime_error);
    }

    SUBCASE("provided arguments construct the value in root with no copies")
    {
        general_tree<LifecycleCounter> gt;
        gt.emplace_root("string999", 999);
        CHECK_EQ(gt.root().data().get_int(), 999);
        CHECK_EQ(gt.root().data().get_string(), "string999");
    }
}