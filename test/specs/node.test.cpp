#define DOCTEST_CONFIG_VOID_CAST_EXPRESSIONS
#include "utils/fixtures/counter.fixture.h"
#include "general-tree.h"
#include "doctest.h"

TEST_SUITE("general_tree::node")
{
	TEST_CASE("right sibling, parent and left child are null by default")
	{
		general_tree<int> tree(1);
		auto node = tree.root();
		CHECK(node.right_sibling().is_null());
		CHECK(node.left_child().is_null());
		CHECK(node.parent().is_null());
	}

	TEST_CASE("equality operator returns true when nodes are the same")
	{
		general_tree<int> tree(1);
		auto root_node = tree.root();
		auto node = tree.insert_left_child(tree.root(), 2);
		REQUIRE_NE(node, root_node);

		root_node = root_node.left_child();
		REQUIRE_EQ(node, root_node);
	}

	TEST_CASE("node::data()")
	{
		SUBCASE("throw invalid argument if node is null")
		{
			general_tree<int> tree;
			CHECK_THROWS_AS(tree.root().data(), std::invalid_argument);
		}
	}

	TEST_CASE("node::child(index)")
	{
		SUBCASE("throw invalid argument if node is null")
		{
			general_tree<int> tree;
			CHECK_THROWS_AS(tree.root().child(2), std::invalid_argument);
		}

		SUBCASE("return the requested child")
		{
			general_tree<int> tree(1);
			auto left = tree.insert_left_child(tree.root(), 2);
			auto right = tree.insert_right_sibling(left, 3);

			REQUIRE_EQ(tree.root().child(0), left);
			REQUIRE_EQ(tree.root().child(1), right);
		}

		SUBCASE("return null node when node has no children")
		{
			general_tree<int> tree(1);
			auto root = tree.root();

			auto child0 = root.child(0);
			REQUIRE(child0.is_null());
		}
	}

	TEST_CASE("node::children_count()")
	{
		SUBCASE("throw invalid argument if node is null")
		{
			general_tree<int> tree;
			CHECK_THROWS_AS(tree.root().children_count(), std::invalid_argument);
		}

		SUBCASE("return 0 if node has no children")
		{
			general_tree<int> tree(1);
			REQUIRE_EQ(tree.root().children_count(), 0);
		}

		SUBCASE("return the number of children")
		{
			general_tree<int> tree(1);
			auto root = tree.root();

			tree.insert_left_child(root, 2);
			tree.insert_right_sibling(root.left_child(), 3);
			tree.insert_right_sibling(root.left_child().right_sibling(), 4);

			REQUIRE_EQ(root.children_count(), 3);
		}
	}

	TEST_CASE("node::depth()")
	{
		SUBCASE("return the depth of the current node")
		{
			general_tree<int> tree(1);
			auto root = tree.root();
			auto child = tree.insert_left_child(root, 2);
			auto grandchild = tree.insert_left_child(child, 3);

			REQUIRE_EQ(child.depth(), 1);
			REQUIRE_EQ(grandchild.depth(), 2);
		}

		SUBCASE("throw invalid argument if node is null")
		{
			general_tree<int> tree;
			CHECK_THROWS_AS(tree.root().depth(), std::invalid_argument);
		}

		SUBCASE("return 0 for root node")
		{
			general_tree<int> tree(42);
			REQUIRE_EQ(tree.root().depth(), 0);
		}
	}

	TEST_CASE("node::descendants_count()")
	{
		SUBCASE("throw invalid argument if node is null")
		{
			general_tree<int> tree;
			CHECK_THROWS_AS(tree.root().descendants_count(), std::invalid_argument);
		}

		SUBCASE("return the number of descendants")
		{
			general_tree<int> tree(1);
			auto root = tree.root();
			auto child1 = tree.insert_left_child(root, 2);             // 1
			auto child2 = tree.insert_right_sibling(child1, 3);        // 2
			auto grandchild1 = tree.insert_left_child(child1, 4);      // 3
			auto grandchild2 = tree.insert_right_sibling(grandchild1, 5); // 4
			tree.insert_left_child(child2, 6);                         // 5

			REQUIRE_EQ(root.descendants_count(), 5);
			REQUIRE_EQ(child1.descendants_count(), 2);
			REQUIRE_EQ(grandchild1.descendants_count(), 0);
		}
	}
}