#include "doctest.h"
#include "general-tree.h"
#include "utils/fixtures/counter.fixture.h"

TEST_SUITE("general_tree::insert_right_sibling(node, tree)")
{
    TEST_CASE("return invalid argument if destiny node is a root")
    {
        general_tree<int> tree(0);
        auto destiny_node = tree.root();
        general_tree<int> new_tree(100);
        REQUIRE_THROWS_AS(tree.insert_right_sibling(destiny_node, new_tree), std::invalid_argument);
    }

    TEST_CASE("return invalid argument if destiny node is nullptr")
    {
        general_tree<int> tree(1);
        auto destiny_node = tree.root().left_child();
        REQUIRE(destiny_node.is_null());

        general_tree<int> new_tree(100);
        REQUIRE_THROWS_AS(tree.insert_right_sibling(destiny_node, new_tree), std::invalid_argument);
    }

    TEST_CASE("return null node if tree is empty")
    {
        general_tree<int> tree(1);
        tree.insert_left_child(tree.root(), 2);

        general_tree<int> empty_tree;
        auto node = tree.insert_right_sibling(tree.root().left_child(), empty_tree);
        REQUIRE(node.is_null());
    }

    TEST_CASE("inserted tree is empty after operation")
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

    TEST_CASE("inserted tree root should be the destiny node right sibling")
    {
        general_tree<int> tree(1);
        auto destiny_node = tree.insert_left_child(tree.root(), 2);

        general_tree<int> new_tree(2);
        auto inserted_tree_root = new_tree.root();

        tree.insert_right_sibling(destiny_node, new_tree);
        REQUIRE_EQ(destiny_node.right_sibling(), inserted_tree_root);
    }

    TEST_CASE("destiny node parent is now the parent of the inserted tree")
    {
        general_tree<int> tree(1);
        auto destiny_node = tree.insert_left_child(tree.root(), 2);
        auto destiny_node_parent = tree.root();

        general_tree<int> new_tree(2);
        auto inserted_tree_root = new_tree.root();

        tree.insert_right_sibling(destiny_node, new_tree);
        REQUIRE_EQ(destiny_node_parent, inserted_tree_root.parent());
    }

    TEST_CASE("return root node of the inserted tree")
    {
        general_tree<int> tree(1);
        tree.insert_left_child(tree.root(), 2);

        general_tree<int> new_tree(2);
        auto inserted_tree_root = new_tree.root();

        auto node = tree.insert_right_sibling(tree.root().left_child(), new_tree);
        REQUIRE_EQ(node, inserted_tree_root);
    }
}

TEST_SUITE("general_tree::emplace_right_sibling(node, ...args)")
{
    TEST_CASE_FIXTURE(CounterFixture, "data is constructed inside node with no copies")
    {
        general_tree<Counter> tree{Counter()};
        tree.insert_left_child(tree.root(), Counter());
        Counter::reset();

        auto node = tree.emplace_right_sibling(tree.root().left_child(), "string", 0);

        CHECK_EQ(Counter::copy_constructor_calls, 0);
        CHECK_EQ(node.data().get_int(), 0);
        CHECK_EQ(node.data().get_string(), "string");
    }
}

TEST_SUITE("general_tree::insert_right_sibling(node, data)")
{
    TEST_CASE_FIXTURE(CounterFixture, "do not make copies when data is an rvalue")
    {
        general_tree<Counter> tree(Counter{});
        auto destiny_node = tree.insert_left_child(tree.root(), Counter{});
        Counter c;
        Counter::reset();
        auto node = tree.insert_right_sibling(destiny_node, std::move(c));

        CHECK_EQ(Counter::copy_constructor_calls, 0);
        CHECK_EQ(Counter::move_constructor_calls, 1);
    }

    TEST_CASE("created node should be the destiny node right sibling")
    {
        general_tree<int> tree(1);
        auto destiny_node = tree.insert_left_child(tree.root(), 2);

        auto node = tree.insert_right_sibling(destiny_node, 3);
        REQUIRE_EQ(destiny_node.right_sibling(), node);
    }

    TEST_CASE("destiny node parent should be the parent of the created node")
    {
        general_tree<int> tree(1);
        auto destiny_node = tree.insert_left_child(tree.root(), 2);

        auto node = tree.insert_right_sibling(destiny_node, 3);
        REQUIRE_EQ(node.parent(), destiny_node.parent());
    }

    TEST_CASE("can not be inserted to root")
    {
        general_tree<int> tree(1);
        auto root = tree.root();

        REQUIRE_THROWS_AS(tree.insert_right_sibling(root, 2), std::invalid_argument);
    }

    TEST_CASE("return the inserted node")
    {
        general_tree<int> tree(1);
        auto destiny_node = tree.insert_left_child(tree.root(), 2);

        auto node = tree.insert_right_sibling(destiny_node, 3);
        REQUIRE_EQ(node.data(), 3);
    }

    TEST_CASE("throw invalid argument if destiny node is null")
    {
        general_tree<int> tree(1);
        auto null_node = tree.root().left_child();
        REQUIRE(null_node.is_null());

        REQUIRE_THROWS_AS(tree.insert_right_sibling(null_node, 2), std::invalid_argument);
    }

    TEST_CASE("throw invalid argument if destiny node is a root")
    {
        general_tree<int> tree(1);
        auto root = tree.root();

        REQUIRE_THROWS_AS(tree.insert_right_sibling(root, 2), std::invalid_argument);
    }
}