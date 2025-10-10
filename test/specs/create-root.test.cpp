#include "doctest.h"
#include "general-tree.h"
#include "utils/fixtures/counter.fixture.h"

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

    TEST_CASE_FIXTURE(CounterFixture, "correct move semantics")
    {
        general_tree<Counter> counters;
        Counter c;
        counters.create_root(std::move(c));
        CHECK_EQ(Counter::copy_constructor_calls, 0);
        CHECK_EQ(Counter::move_constructor_calls, 1);
    }

    TEST_CASE_FIXTURE(CounterFixture, "temporary value triggers move")
    {
        general_tree<Counter> gt;
        gt.create_root(Counter{});
        CHECK_EQ(Counter::copy_constructor_calls, 0);
        CHECK_EQ(Counter::move_constructor_calls, 1);
    }
}

TEST_SUITE("general_tree::emplace_root(...args)")
{
    TEST_CASE_FIXTURE(CounterFixture, "does not make copies")
    {
        general_tree<Counter> counters;
        counters.emplace_root("string", 0);
        CHECK_EQ(counters.root().data().get_int(), 0);
        CHECK_EQ(counters.root().data().get_string(), "string");
        REQUIRE_EQ(Counter::copy_constructor_calls, 0);
    }

    TEST_CASE("throw runtime error if root already exists")
    {
        general_tree<int> empty_tree;
        empty_tree.emplace_root(1);
        CHECK_THROWS_AS(empty_tree.create_root(2), std::runtime_error);
    }

    TEST_CASE("data is stored in root node")
    {
        general_tree<int> empty_tree;
        const int value = 123;
        empty_tree.emplace_root(value);
        REQUIRE_EQ(empty_tree.root().data(), value);
    }
}
