#include "doctest.h"
#include "general-tree.h"
#include "utils/fixtures/lifecycle-counter.fixture.h"
#include "utils/helpers/seed-tree.h"

TEST_CASE_FIXTURE(LifecycleCounterFixture, "assigment operator")
{
    SUBCASE("no error in self-assigning")
    {
        general_tree<int> gt(1);
        gt = gt;
        REQUIRE_EQ(gt.root().data(), 1);
    }

    SUBCASE("current tree should remain empty if other tree is empty")
    {
        general_tree<LifecycleCounter> gt = seed_tree(5);
        general_tree<LifecycleCounter> other;

        gt = other;

        REQUIRE(gt.empty());
    }

    SUBCASE("should not modify the other tree")
    {
        general_tree<LifecycleCounter> other = seed_tree(10);
        auto backup = other;

        general_tree<LifecycleCounter> gt(LifecycleCounter("string0", 0));

        gt = other;

        REQUIRE_EQ(other, backup);
    }

    SUBCASE("create as many copies as elements the other tree contains")
    {
        const std::size_t gt_tree_size = 13;
        general_tree<LifecycleCounter> gt = seed_tree(gt_tree_size);
        general_tree<LifecycleCounter> new_tree(LifecycleCounter("string1", 1));

        new_tree = gt;

        REQUIRE_EQ(LifecycleCounter::copy_constructor_calls, gt_tree_size);
    }

    SUBCASE("previous elements should be destroyed")
    {
        std::size_t gt_size = 9;
        general_tree<LifecycleCounter> gt = seed_tree(gt_size);
        general_tree<LifecycleCounter> gt2;

        gt = gt2;

        REQUIRE_EQ(LifecycleCounter::destructor_calls, 9);
    }

    SUBCASE("trees should be equal after operation")
    {
        general_tree<LifecycleCounter> gt = seed_tree(13);
        general_tree<LifecycleCounter> gt2(LifecycleCounter("string1", 1));

        gt2 = gt;

        REQUIRE_EQ(gt, gt2);
    }
}

TEST_CASE_FIXTURE(LifecycleCounterFixture, "equality operator")
{
    SUBCASE("return true if tree is the same tree")
    {
        general_tree<int> gt(1);
        REQUIRE(gt == gt);
    }

    SUBCASE("return false if other tree is empty")
    {
        general_tree<int> gt(1);
        gt.insert_left_child(gt.root(), 3);

        general_tree<int> other;
        REQUIRE_FALSE(gt == other);
    }

    SUBCASE("return false if current tree is empty")
    {
        general_tree<int> gt_empty;
        general_tree<int> other(1);
        other.insert_left_child(other.root(), 3);

        REQUIRE_FALSE(gt_empty == other);
    }

    SUBCASE("return true if both empty")
    {
        general_tree<int> gt_empty1;
        general_tree<int> gt_empty2;
        REQUIRE(gt_empty1 == gt_empty2);
    }

    SUBCASE("return true if trees contains the same values")
    {
        general_tree<LifecycleCounter> gt = seed_tree(10);
        general_tree<LifecycleCounter> other = seed_tree(10);
        REQUIRE(gt == other);
    }

    SUBCASE("return false if trees have different length")
    {
        general_tree<LifecycleCounter> gt = seed_tree(10);
        general_tree<LifecycleCounter> other = seed_tree(11);
        REQUIRE_FALSE(gt == other);
    }

    SUBCASE("return false if trees are root only and contain the same values")
    {
        general_tree<int> gt1(1);
        general_tree<int> gt2(2);
        REQUIRE_FALSE(gt1 == gt2);
    }
}
