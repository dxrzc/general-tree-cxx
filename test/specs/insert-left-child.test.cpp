#include "doctest.h"
#include "general-tree.h"
#include "utils/fixtures/lifecycle-counter.fixture.h"
#include "utils/helpers/seed-tree.h"

TEST_CASE_FIXTURE(LifecycleCounterFixture, "insert_left_child")
{
    SUBCASE("make one copy of the provided value and insert it as left child")
    {
        general_tree<LifecycleCounter> gt = seed_tree(3);
        LifecycleCounter value("string1", 1);
        gt.insert_left_child(gt.root().left_child(), value);
        CHECK_EQ(LifecycleCounter::copy_constructor_calls, 1);
        CHECK_EQ(LifecycleCounter::parameterized_constructor_calls, 1);
        CHECK_EQ(gt.root().left_child().data(), value);
    }

    SUBCASE("destiny node is now the parent of the inserted node")
    {
        general_tree<int> tree(1);
        auto node = tree.insert_left_child(tree.root(), 100);
        REQUIRE_EQ(node.parent().data(), tree.root().data());
    }

    SUBCASE("throw invalid argument if destiny node is null")
    {
        general_tree<int> tree(1);
        auto null_dest_node = tree.root().left_child();
        REQUIRE(null_dest_node.is_null());
        REQUIRE_THROWS_AS(tree.insert_left_child(null_dest_node, tree), std::invalid_argument);
    }

    SUBCASE("return the created node")
    {
        general_tree<int> tree(1);
        const int new_value = 83190831;
        auto node = tree.insert_left_child(tree.root(), new_value);
        REQUIRE_EQ(node.data(), new_value);
    }
}

TEST_CASE_FIXTURE(LifecycleCounterFixture, "insert_left_child (move)")
{
    SUBCASE("do not make copies of the provided value and insert it as left child")
    {
        general_tree<LifecycleCounter> gt = seed_tree(3);
        LifecycleCounter value("string1", 1);
        gt.insert_left_child(gt.root(), std::move(value));

        CHECK_EQ(LifecycleCounter::copy_constructor_calls, 0);
        CHECK_EQ(LifecycleCounter::parameterized_constructor_calls, 1);
        CHECK_EQ(LifecycleCounter::move_constructor_calls, 1);

        CHECK_EQ(gt.root().left_child().data().get_int(), 1);
        CHECK_EQ(gt.root().left_child().data().get_string(), "string1");
    }

    SUBCASE("temporary value triggers move")
    {
        general_tree<LifecycleCounter> gt = seed_tree(1);
        gt.insert_left_child(gt.root(), LifecycleCounter{});

        CHECK_EQ(LifecycleCounter::copy_constructor_calls, 0);
        CHECK_EQ(LifecycleCounter::move_constructor_calls, 1);
    }
}

TEST_CASE_FIXTURE(LifecycleCounterFixture, "insert_left_child (emplacement)")
{
    SUBCASE("use the arguments to construct the value as the left child")
    {
        general_tree<LifecycleCounter> gt = seed_tree(1);
        gt.emplace_left_child(gt.root(), "string100", 100);

        CHECK_EQ(gt.root().left_child().data().get_int(), 100);
        CHECK_EQ(gt.root().left_child().data().get_string(), "string100");

        CHECK_EQ(LifecycleCounter::copy_constructor_calls, 0);
        CHECK_EQ(LifecycleCounter::move_constructor_calls, 0);
        CHECK_EQ(LifecycleCounter::parameterized_constructor_calls, 1);
    }
}

TEST_CASE_FIXTURE(LifecycleCounterFixture, "insert_left_child (tree)")
{
    SUBCASE("destiny node is now the parent of the inserted tree root")
    {
        general_tree<int> tree(1);
        general_tree<int> new_tree(999);
        auto inserted_tree_root = new_tree.root();

        tree.insert_left_child(tree.root(), new_tree);
        REQUIRE_EQ(inserted_tree_root.parent(), tree.root());
    }

    SUBCASE("moved-from tree is empty after operation")
    {
        general_tree<int> tree(1);
        general_tree<int> new_tree(1);
        new_tree.insert_left_child(new_tree.root(), 2);
        new_tree.insert_left_child(new_tree.root().left_child(), 3);
        REQUIRE_FALSE(new_tree.empty());

        tree.insert_left_child(tree.root(), new_tree);
        REQUIRE(new_tree.empty());
    }

    SUBCASE("return the root node of the inserted tree")
    {
        general_tree<int> tree(1);
        general_tree<int> new_tree(101);
        auto inserted_tree_root = new_tree.root();

        auto node = tree.insert_left_child(tree.root(), new_tree);
        REQUIRE_EQ(node, inserted_tree_root);
    }

    SUBCASE("throw invalid argument if destiny node is null")
    {
        general_tree<int> tree(1);
        general_tree<int> new_tree(1);
        auto null_dest_node = tree.root().left_child();
        REQUIRE(null_dest_node.is_null());
        REQUIRE_THROWS_AS(tree.insert_left_child(null_dest_node, new_tree), std::invalid_argument);
    }

    SUBCASE("return null node if inserted tree is empty")
    {
        general_tree<int> tree(1);
        general_tree<int> empty_tree;
        auto node = tree.insert_left_child(tree.root(), empty_tree);
        REQUIRE(node.is_null());
    }

    SUBCASE("throw invalid argument if tree is inserted to its own root")
    {
        general_tree<int> tree(1);
        REQUIRE_THROWS_AS(tree.insert_left_child(tree.root(), tree), std::invalid_argument);
    }
}