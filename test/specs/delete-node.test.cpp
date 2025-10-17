#include "general-tree.h"
#include "utils/fixtures/lifecycle-counter.fixture.h"
#include <doctest.h>
#include <stdexcept>

TEST_CASE_FIXTURE(LifecycleCounterFixture, "delete left child")
{
    SUBCASE("delete left child and all the nodes hanging from it")
    {
        general_tree<LifecycleCounter> gt;
        gt.emplace_root("string1", 0);
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
        general_tree<LifecycleCounter> gt;
        gt.emplace_root("string1", 0);
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

    SUBCASE("throw invalid argument if node is null")
    {
        general_tree<int> gt;
        const auto node = gt.root();
        REQUIRE(node.is_null());
        REQUIRE_THROWS_AS(gt.delete_left_child(node), std::invalid_argument);
    }

    SUBCASE("no error if node does not have a left child")
    {
        general_tree<int> gt(1);
        const auto node = gt.root();
        REQUIRE(node.left_child().is_null());
        CHECK_NOTHROW(gt.delete_left_child(node));
    }

    SUBCASE("make the next children the new left child")
    {
        general_tree<int> gt(1);
        const auto next_child = gt.insert_left_child(gt.root(), 1920121);
        gt.insert_left_child(gt.root(), 389813); // deleted

        gt.delete_left_child(gt.root());

        REQUIRE_EQ(gt.root().left_child(), next_child);
    }

    SUBCASE("children count is decreased")
    {
        general_tree<int> gt(1);
        gt.insert_left_child(gt.root(), 1);
        gt.insert_left_child(gt.root(), 2);
        gt.insert_left_child(gt.root(), 3);
        gt.insert_left_child(gt.root(), 4);

        gt.delete_left_child(gt.root());

        REQUIRE_EQ(gt.root().children_count(), 3);
    }
}

TEST_CASE_FIXTURE(LifecycleCounterFixture, "delete right sibling")
{
    SUBCASE("delete right sibling and all the nodes hanging from it")
    {
        general_tree<LifecycleCounter> gt;
        gt.emplace_root("string1", 1);
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
        general_tree<LifecycleCounter> gt;
        gt.emplace_root("string1", 0);
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

    SUBCASE("throw invalid argument if node is null")
    {
        general_tree<int> gt(1);
        const auto node = gt.root().left_child();
        REQUIRE(node.is_null());
        REQUIRE_THROWS_AS(gt.delete_right_sibling(node), std::invalid_argument);
    }

    SUBCASE("no error if node does not have a right sibling")
    {
        general_tree<int> gt(1);
        const auto node = gt.insert_left_child(gt.root(), 2);
        CHECK(node.right_sibling().is_null());
        CHECK_NOTHROW(gt.delete_right_sibling(node));
    }

    SUBCASE("throw invalid argument if node is a root node")
    {
        general_tree<int> gt(1);
        gt.insert_left_child(gt.root(), 2);
        REQUIRE_THROWS_AS(gt.delete_right_sibling(gt.root()), std::invalid_argument);
    }

    SUBCASE("make the next sibling the right sibling")
    {
        general_tree<int> gt(1);
        const auto node = gt.insert_left_child(gt.root(), 2);

        const auto next_sibling = gt.insert_right_sibling(node, 3);
        gt.insert_right_sibling(node, 4);

        gt.delete_right_sibling(node);

        REQUIRE_EQ(node.right_sibling(), next_sibling);
    }

    SUBCASE("parent's children count is decreased")
    {
        general_tree<int> gt(1);
        gt.insert_left_child(gt.root(), 1);
        gt.insert_left_child(gt.root(), 2);
        gt.insert_left_child(gt.root(), 3);
        gt.insert_left_child(gt.root(), 4);

        gt.delete_right_sibling(gt.root().left_child());

        REQUIRE_EQ(gt.root().children_count(), 3);
    }
}
