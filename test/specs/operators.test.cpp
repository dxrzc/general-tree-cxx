#include "doctest.h"
#include "general-tree.h"
#include "utils/helpers/seed-tree.h"

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