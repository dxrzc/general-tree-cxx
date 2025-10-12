#include "doctest.h"
#include "general-tree.h"
#include "utils/fixtures/lifecycle-counter.fixture.h"
#include "utils/helpers/seed-tree.h"

TEST_CASE_FIXTURE(LifecycleCounterFixture, "insert_right_sibling")
{
    SUBCASE("create one copy of the provided value and insert it as right sibling")
    {
        general_tree<LifecycleCounter> gt = seed_tree(2);
        LifecycleCounter value("string00", 0);
        gt.insert_right_sibling(gt.root().left_child(), value);

        CHECK_EQ(gt.root().left_child().right_sibling().data(), value);
        CHECK_EQ(LifecycleCounter::copy_constructor_calls, 1);
        CHECK_EQ(LifecycleCounter::parameterized_constructor_calls, 1);
    }

    SUBCASE("created node should be the destiny node right sibling")
    {
        general_tree<int> tree(1);
        auto destiny_node = tree.insert_left_child(tree.root(), 2);

        auto node = tree.insert_right_sibling(destiny_node, 3);
        REQUIRE_EQ(destiny_node.right_sibling(), node);
    }

    SUBCASE("destiny node parent should be the parent of the created node")
    {
        general_tree<int> tree(1);
        auto destiny_node = tree.insert_left_child(tree.root(), 2);

        auto node = tree.insert_right_sibling(destiny_node, 3);
        REQUIRE_EQ(node.parent(), destiny_node.parent());
    }

    SUBCASE("return the inserted node")
    {
        general_tree<int> tree(1);
        auto destiny_node = tree.insert_left_child(tree.root(), 2);

        auto node = tree.insert_right_sibling(destiny_node, 3);
        REQUIRE_EQ(node.data(), 3);
    }

    SUBCASE("throw invalid argument if destiny node is null")
    {
        general_tree<int> tree(1);
        auto null_node = tree.root().left_child();
        REQUIRE(null_node.is_null());

        REQUIRE_THROWS_AS(tree.insert_right_sibling(null_node, 2), std::invalid_argument);
    }

    SUBCASE("throw invalid argument if destiny node is a root")
    {
        general_tree<int> tree(1);
        auto root = tree.root();

        REQUIRE_THROWS_AS(tree.insert_right_sibling(root, 2), std::invalid_argument);
    }
}

TEST_CASE_FIXTURE(LifecycleCounterFixture, "insert_right_sibling (move)")
{
    SUBCASE("do not create copies of the provided value and insert it as right sibling")
    {
        general_tree<LifecycleCounter> gt = seed_tree(2);
        LifecycleCounter value("string09", 9);
        gt.insert_right_sibling(gt.root().left_child(), std::move(value));

        CHECK_EQ(gt.root().left_child().right_sibling().data().get_int(), 9);
        CHECK_EQ(gt.root().left_child().right_sibling().data().get_string(), "string09");

        CHECK_EQ(LifecycleCounter::copy_constructor_calls, 0);
        CHECK_EQ(LifecycleCounter::move_constructor_calls, 1);
        CHECK_EQ(LifecycleCounter::parameterized_constructor_calls, 1);
    }

    SUBCASE("temporary value triggers move")
    {
        general_tree<LifecycleCounter> gt = seed_tree(2);
        gt.insert_right_sibling(gt.root().left_child(), LifecycleCounter{});
        CHECK_EQ(LifecycleCounter::copy_constructor_calls, 0);
        CHECK_EQ(LifecycleCounter::move_constructor_calls, 1);
    }
}

TEST_CASE_FIXTURE(LifecycleCounterFixture, "insert_right_sibling (emplacement)")
{
    SUBCASE("use the arguments to construct the value as the left child with no copies") 
    {
        general_tree<LifecycleCounter> gt = seed_tree(2);
        gt.emplace_right_sibling(gt.root().left_child(), "string11", 11);

        CHECK_EQ(gt.root().left_child().right_sibling().data().get_int(), 11);
        CHECK_EQ(gt.root().left_child().right_sibling().data().get_string(), "string11");

        CHECK_EQ(LifecycleCounter::copy_constructor_calls, 0);
        CHECK_EQ(LifecycleCounter::parameterized_constructor_calls, 1);
    }
}

TEST_CASE_FIXTURE(LifecycleCounterFixture, "insert_right_sibling (tree)") 
{
    SUBCASE("throw invalid argument if destiny node is a root")
    {
        general_tree<int> tree(0);
        auto destiny_node = tree.root();
        general_tree<int> new_tree(100);
        REQUIRE_THROWS_AS(tree.insert_right_sibling(destiny_node, new_tree), std::invalid_argument);
    }

    SUBCASE("throw invalid argument if destiny node is nullptr")
    {
        general_tree<int> tree(1);
        auto destiny_node = tree.root().left_child();
        REQUIRE(destiny_node.is_null());

        general_tree<int> new_tree(100);
        REQUIRE_THROWS_AS(tree.insert_right_sibling(destiny_node, new_tree), std::invalid_argument);
    }

    SUBCASE("return null node if tree is empty")
    {
        general_tree<int> tree(1);
        tree.insert_left_child(tree.root(), 2);

        general_tree<int> empty_tree;
        auto node = tree.insert_right_sibling(tree.root().left_child(), empty_tree);
        REQUIRE(node.is_null());
    }

    SUBCASE("inserted tree is empty after operation")
    {
        general_tree<int> tree(1);
        tree.insert_left_child(tree.root(), 2);

        general_tree<int> new_tree(2);
        new_tree.insert_left_child(new_tree.root(), 3);
        new_tree.insert_left_child(new_tree.root().left_child(), 4);
        REQUIRE_FALSE(new_tree.empty());

        tree.insert_right_sibling(tree.root().left_child(), new_tree);
        REQUIRE(new_tree.empty());
    }

    SUBCASE("inserted tree root should be the right sibling of the destiny node")
    {
        general_tree<int> tree(1);
        auto destiny_node = tree.insert_left_child(tree.root(), 2);

        general_tree<int> new_tree(2);
        auto inserted_tree_root = new_tree.root();

        tree.insert_right_sibling(destiny_node, new_tree);
        REQUIRE_EQ(destiny_node.right_sibling(), inserted_tree_root);
    }

    SUBCASE("destiny node parent should be the parent of the inserted tree root node")
    {
        general_tree<int> tree(1);
        auto destiny_node = tree.insert_left_child(tree.root(), 2);
        auto destiny_node_parent = tree.root();

        general_tree<int> new_tree(2);
        auto inserted_tree_root = new_tree.root();

        tree.insert_right_sibling(destiny_node, new_tree);
        REQUIRE_EQ(destiny_node_parent, inserted_tree_root.parent());
    }

    SUBCASE("return root node of the inserted tree")
    {
        general_tree<int> tree(1);
        tree.insert_left_child(tree.root(), 2);

        general_tree<int> new_tree(2);
        auto inserted_tree_root = new_tree.root();

        auto node = tree.insert_right_sibling(tree.root().left_child(), new_tree);
        REQUIRE_EQ(node, inserted_tree_root);
    }
}