#include "general-tree.h"
#include "utils/fixtures/lifecycle-counter.fixture.h"
#include <doctest.h>

TEST_CASE_FIXTURE(LifecycleCounterFixture, "delete left child")
{
    SUBCASE("delete left child and all the nodes hanging from it")
    {
        general_tree<LifecycleCounter> gt("string1", 0);
        gt.emplace_left_child(gt.root(), "string2", 2);
        gt.emplace_left_child(gt.root().left_child(), "string3", 3);
        gt.emplace_left_child(gt.root().left_child().left_child(), "string5", 5);
        gt.emplace_left_child(gt.root().left_child().left_child(), "string6", 6);

        gt.delete_left_child(gt.root());

        CHECK_EQ(LifecycleCounter::destructor_calls, 4);
        CHECK(gt.root().left_child().is_null());
    }

    SUBCASE("insert nodes after delete is safe and new nodes are correctly destroyed")
    {
        general_tree<LifecycleCounter> gt("string1", 0);
        gt.emplace_left_child(gt.root(), "string2", 2);
        // delete
        gt.delete_left_child(gt.root());
        LifecycleCounter::reset();
        // insert new nodes
        gt.emplace_left_child(gt.root(), "string22", 22);
        gt.emplace_left_child(gt.root().left_child(), "string33", 33);

        // should be deleted, root and two nodes
        gt.clear();
        CHECK_EQ(LifecycleCounter::destructor_calls, 3);
    }
}

TEST_CASE_FIXTURE(LifecycleCounterFixture, "delete right sibling")
{
    SUBCASE("delete right sibling and all the nodes hanging from it")
    {
        general_tree<LifecycleCounter> gt("string1", 0);
        const auto child1 = gt.emplace_left_child(gt.root(), "string2", 2);
        // create and populate right sibling
        const auto right_sibling = gt.emplace_right_sibling(child1, "string3", 3);
        gt.emplace_left_child(right_sibling, "string4", 4);
        gt.emplace_left_child(right_sibling, "string5", 5);
        gt.emplace_left_child(right_sibling.left_child(), "string5", 5);

        gt.delete_right_sibling(gt.root().left_child());

        CHECK_EQ(LifecycleCounter::destructor_calls, 4);
        CHECK(gt.root().left_child().right_sibling().is_null());
    }

    SUBCASE("insert nodes after delete is safe and new nodes are correctly destroyed")
    {
        general_tree<LifecycleCounter> gt("string1", 0);
        const auto child1 = gt.emplace_left_child(gt.root(), "string2", 2);
        gt.emplace_right_sibling(child1, "string3", 3);

        // delete
        gt.delete_right_sibling(gt.root().left_child());
        LifecycleCounter::reset();

        // insert new right sibling branch
        gt.emplace_right_sibling(child1, "string4", 4);
        gt.emplace_left_child(child1.right_sibling(), "string5", 5);

        // should delete root, its child and right sibling branch
        gt.clear();
        CHECK_EQ(LifecycleCounter::destructor_calls, 4);
    }
}
