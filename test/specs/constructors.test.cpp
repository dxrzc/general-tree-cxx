#include "doctest.h"
#include "general-tree.h"
#include "utils/fixtures/counter.fixture.h"
#include <utility>

TEST_SUITE("general_tree::general_tree()")
{
    TEST_CASE("tree is empty by default")
    {
        general_tree<int> gt;
        REQUIRE(gt.empty());
    }
}

TEST_SUITE("general_tree::general_tree(root_value)")
{
    TEST_CASE("root_value is now the root")
    {
        const int root_value = 100201;
        general_tree<int> gt(root_value);
        REQUIRE_EQ(gt.root().data(), root_value);
    }

    TEST_CASE_FIXTURE(CounterFixture, "correct move semantics")
    {
        Counter counter;
        general_tree<Counter> gt(std::move(counter));
        CHECK_EQ(Counter::move_constructor_calls, 1);
        CHECK_EQ(Counter::copy_constructor_calls, 0);
    }

    TEST_CASE_FIXTURE(CounterFixture, "construction from temporary value triggers move")
    {
        general_tree<Counter> gt(Counter{});
        CHECK_EQ(Counter::move_constructor_calls, 1);
        CHECK_EQ(Counter::copy_constructor_calls, 0);
    }
}

TEST_SUITE("general_tree::general_tree(...args)")
{
    TEST_CASE_FIXTURE(CounterFixture, "create root and do not make copies")
    {
        general_tree<Counter> gt("string123", 100);
        CHECK_EQ(gt.root().data().get_string(), "string123");
        CHECK_EQ(gt.root().data().get_int(), 100);
    }
}

TEST_SUITE("general_tree::general_tree(general_tree&& rhs)")
{
    TEST_CASE_FIXTURE(CounterFixture, "do not make any copies or movements")
    {
        general_tree<Counter> counters;
        counters.emplace_root("string", 0);

        general_tree<Counter> new_tree(std::move(counters));
        CHECK_EQ(Counter::copy_constructor_calls, 0);
        CHECK_EQ(Counter::move_constructor_calls, 0);
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

    TEST_CASE("no errors if rhs tree is empty")
    {
        general_tree<int> empty;
        general_tree<int> gt(std::move(empty));
    }

    TEST_CASE("moved-from tree can be reused normally afterwards")
    {
        general_tree<int> movedfrom(0);
        movedfrom.insert_left_child(movedfrom.root(), 1);
        movedfrom.insert_right_sibling(movedfrom.root().left_child(), 2);
        general_tree<int> gt(std::move(movedfrom));

        movedfrom.create_root(1);
        movedfrom.insert_left_child(movedfrom.root(), 2);
        movedfrom.insert_right_sibling(movedfrom.root().left_child(), 3);
        movedfrom.insert_left_child(movedfrom.root(), 4);
    }
}
