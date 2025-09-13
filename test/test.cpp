#define DOCTEST_CONFIG_VOID_CAST_EXPRESSIONS
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
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

	TEST_CASE("node::data() -- throw invalid argument if node is null")
	{
		general_tree<int> tree;
		CHECK_THROWS_AS(tree.root().data(), std::invalid_argument);
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

