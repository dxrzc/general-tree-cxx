#define DOCTEST_CONFIG_VOID_CAST_EXPRESSIONS
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "utils/fixtures/counter.fixture.h"
#include "general-tree.h"
#include "doctest.h"

TEST_SUITE("general_tree::constructors")
{
	TEST_CASE_FIXTURE(CounterFixture, "do not make any copies or movements")
	{
		general_tree<Counter> counters;
		counters.emplace_root("string", 0);

		general_tree<Counter> new_tree(std::move(counters));
		REQUIRE_EQ(Counter::copy_constructor_calls, 0);
		REQUIRE_EQ(Counter::move_constructor_calls, 0);
	}

	TEST_CASE("rhs tree remains empty after operation")
	{
		general_tree<int> rhs(1);
		rhs.insert_left_child(rhs.root(), 2);
		rhs.insert_right_sibling(rhs.root().left_child(), 3);

		general_tree<int> tree(std::move(rhs));
		REQUIRE(rhs.empty());
	}

	TEST_CASE("new tree contains all the nodes of the rhs tree")
	{
		general_tree<int> rhs(1);
		rhs.insert_left_child(rhs.root(), 2);
		rhs.insert_right_sibling(rhs.root().left_child(), 3);

		auto root_node = rhs.root();

		general_tree<int> tree(std::move(rhs));
		REQUIRE_EQ(tree.root(), root_node);
	}
}

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

TEST_SUITE("general_tree::create_root(data)")
{
	TEST_CASE("throw runtime error if root already exists")
	{
		general_tree<int> empty_tree;
		empty_tree.create_root(1);
		CHECK_THROWS_AS(empty_tree.create_root(2), std::runtime_error);
	}

	TEST_CASE("data is stored in root node")
	{
		general_tree<int> empty_tree;
		const int value = 123;
		empty_tree.create_root(value);
		REQUIRE_EQ(empty_tree.root().data(), value);
	}

	TEST_CASE_FIXTURE(CounterFixture, "do not make copies when data is an rvalue")
	{
		general_tree<Counter> counters;
		Counter c;
		counters.create_root(std::move(c));
		CHECK_EQ(Counter::copy_constructor_calls, 0);
		CHECK_EQ(Counter::move_constructor_calls, 1);
	}
}

TEST_SUITE("general_tree::emplace_root(...args)")
{
	TEST_CASE_FIXTURE(CounterFixture, "data is constructed inside node with no copies")
	{
		general_tree<Counter> counters;
		counters.emplace_root("string", 0);
		CHECK_EQ(counters.root().data().get_int(), 0);
		CHECK_EQ(counters.root().data().get_string(), "string");
		REQUIRE_EQ(Counter::copy_constructor_calls, 0);
	}
}

TEST_SUITE("general_tree:insert_left_child(node, data)")
{
	TEST_CASE_FIXTURE(CounterFixture, "do not create copies when data is an rvalue")
	{
		general_tree<Counter> counter_tree{ Counter() };
		Counter::reset();
		Counter c;
		counter_tree.insert_left_child(counter_tree.root(), std::move(c));
		CHECK_EQ(Counter::copy_constructor_calls, 0);
		CHECK_EQ(Counter::move_constructor_calls, 1);
	}

	TEST_CASE("throw invalid argument if destiny node is null")
	{
		general_tree<int> tree(1);
		auto null_dest_node = tree.root().left_child();
		REQUIRE(null_dest_node.is_null());
		REQUIRE_THROWS_AS(tree.insert_left_child(null_dest_node, tree), std::invalid_argument);
	}

	TEST_CASE("data is stored in the left child of the destiny node")
	{
		general_tree<int> tree(1);
		const int new_value = 12121;
		tree.insert_left_child(tree.root(), new_value);
		REQUIRE_EQ(tree.root().left_child().data(), new_value);
	}

	TEST_CASE("return the created node")
	{
		general_tree<int> tree(1);
		const int new_value = 83190831;
		auto node = tree.insert_left_child(tree.root(), new_value);
		REQUIRE_EQ(node.data(), new_value);
	}

	TEST_CASE("destiny node is now the parent of the inserted node")
	{
		general_tree<int> tree(1);
		auto node = tree.insert_left_child(tree.root(), 100);
		REQUIRE_EQ(node.parent().data(), tree.root().data());
	}
}

TEST_SUITE("general_tree::insert_left_child(node, tree)")
{
	TEST_CASE("destiny node is now the parent of the inserted tree")
	{
		general_tree<int> tree(1);
		general_tree<int> new_tree(999);
		auto inserted_tree_root = new_tree.root();

		tree.insert_left_child(tree.root(), new_tree);
		REQUIRE_EQ(inserted_tree_root.parent(), tree.root());
	}

	TEST_CASE("inserted tree is empty after operation")
	{
		general_tree<int> tree(1);
		general_tree<int> new_tree(1);
		new_tree.insert_left_child(new_tree.root(), 2);
		new_tree.insert_left_child(new_tree.root().left_child(), 3);
		REQUIRE_FALSE(new_tree.empty());

		tree.insert_left_child(tree.root(), new_tree);
		REQUIRE(new_tree.empty());
	}

	TEST_CASE("return the root node of the inserted tree")
	{
		general_tree<int> tree(1);
		general_tree<int> new_tree(101);
		auto inserted_tree_root = new_tree.root();

		auto node = tree.insert_left_child(tree.root(), new_tree);
		REQUIRE_EQ(node, inserted_tree_root);
	}

	TEST_CASE("throw invalid argument if destiny node is null")
	{
		general_tree<int> tree(1);
		general_tree<int> new_tree(1);
		auto null_dest_node = tree.root().left_child();
		REQUIRE(null_dest_node.is_null());
		REQUIRE_THROWS_AS(
			tree.insert_left_child(null_dest_node, new_tree),
			std::invalid_argument
		);
	}

	TEST_CASE("return null node if inserted tree is empty")
	{
		general_tree<int> tree(1);
		general_tree<int> empty_tree;
		auto node = tree.insert_left_child(tree.root(), empty_tree);
		REQUIRE(node.is_null());
	}

	TEST_CASE("throw invalid argument if tree is inserted to its own root")
	{
		general_tree<int> tree(1);
		REQUIRE_THROWS_AS(
			tree.insert_left_child(tree.root(), tree), std::invalid_argument);
	}
}

TEST_SUITE("general_tree::emplace_left_child(...args)")
{
	TEST_CASE_FIXTURE(CounterFixture, "data is constructed inside node with no copies")
	{
		general_tree<Counter> tree{ Counter() };
		Counter::reset();

		auto node = tree.emplace_left_child(tree.root(), "string", 0);

		CHECK_EQ(Counter::copy_constructor_calls, 0);
		CHECK_EQ(node.data().get_int(), 0);
		CHECK_EQ(node.data().get_string(), "string");
	}
}

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
		general_tree<Counter> tree{ Counter() };
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