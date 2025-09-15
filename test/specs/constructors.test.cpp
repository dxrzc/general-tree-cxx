#include "utils/fixtures/counter.fixture.h"
#include "general-tree.h"
#include "doctest.h"

TEST_SUITE("general_tree::general_tree(general_tree&&)")
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