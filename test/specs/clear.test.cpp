#include "utils/fixtures/counter.fixture.h"
#include "general-tree.h"
#include "doctest.h"

TEST_SUITE("general_tree::clear()")
{
	TEST_CASE_FIXTURE(CounterFixture, "delete all the nodes from the tree")
	{
		general_tree<Counter> tree;

		auto root = tree.emplace_root("root", 1);
		auto c1 = tree.emplace_left_child(root, "left", 2);
		auto c2 = tree.emplace_right_sibling(c1, "right", 3);
		tree.emplace_left_child(c1, "left-left", 4);
		tree.emplace_left_child(c2, "right-left", 5);

		REQUIRE_EQ(Counter::copy_constructor_calls, 0);
		REQUIRE_EQ(Counter::move_constructor_calls, 0);
		REQUIRE_EQ(Counter::copy_assignment_calls, 0);
		REQUIRE_EQ(Counter::move_assignment_calls, 0);

		tree.clear();

		REQUIRE_EQ(Counter::destructor_calls, 5);
	}

	TEST_CASE("tree is empty after clear")
	{
		general_tree<int> tree(0);
		tree.insert_left_child(tree.root(), 1);
		tree.insert_left_child(tree.root(), 2);
		tree.insert_right_sibling(tree.root().left_child(), 3);
		tree.clear();
		REQUIRE(tree.empty());
	}

	TEST_CASE("calling clear() on an already empty tree is safe")
	{
		general_tree<int> tree;
		REQUIRE(tree.root().is_null());

		CHECK_NOTHROW(tree.clear());
		REQUIRE(tree.root().is_null());
	}
}