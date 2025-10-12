#include "doctest.h"
#include "general-tree.h"
#include "utils/fixtures/lifecycle-counter.fixture.h"
#include "utils/helpers/seed-tree.h"

TEST_CASE_FIXTURE(LifecycleCounterFixture, "clear")
{
    SUBCASE("delete all nodes from tree")
    {
        std::size_t gt_size = 10;
        general_tree<LifecycleCounter> gt = seed_tree(gt_size);
        gt.clear();
        REQUIRE_EQ(LifecycleCounter::destructor_calls, gt_size);
    }

    SUBCASE("tree is empty after clear")
    {
        general_tree<LifecycleCounter> gt = seed_tree(5);
        gt.clear();
        REQUIRE(gt.empty());
    }

    SUBCASE("calling clear on an empty tree is safe")
    {
        general_tree<int> emptygt;
        REQUIRE(emptygt.root().is_null());
        REQUIRE_NOTHROW(emptygt.clear());
        REQUIRE(emptygt.root().is_null());
    }

    SUBCASE("inserting new nodes after clear is safe")
    {
        general_tree<LifecycleCounter> gt = seed_tree(5);
        gt.clear();
        // reset
        LifecycleCounter::destructor_calls = 0;

        gt.emplace_root("string", 0);
        gt.emplace_left_child(gt.root(), "string1", 1);
        gt.emplace_right_sibling(gt.root().left_child(), "string2", 2);
        gt.clear();
        // new nodes destroyed successfully
        REQUIRE_EQ(LifecycleCounter::destructor_calls, 3);
    }
}