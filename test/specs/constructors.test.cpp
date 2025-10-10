#include "doctest.h"
#include "general-tree.h"
#include "utils/fixtures/lifecycle-counter.fixture.h"
#include "utils/helpers/seed-tree.h"
#include <utility>

TEST_CASE("default constructor")
{
    SUBCASE("tree is empty")
    {
        general_tree<int> gt;
        REQUIRE(gt.empty());
    }
}

TEST_CASE_FIXTURE(LifecycleCounterFixture, "root value construction (move)")
{
    SUBCASE("provided value is stored in root with no copies")
    {
        LifecycleCounter value("string100", 100);
        general_tree<LifecycleCounter> gt(std::move(value));
        CHECK_EQ(gt.root().data().get_int(), 100);
        CHECK_EQ(gt.root().data().get_string(), "string100");
        CHECK_EQ(LifecycleCounter::move_constructor_calls, 1);
        CHECK_EQ(LifecycleCounter::copy_constructor_calls, 0);        
    }

    SUBCASE("construction from temporary value triggers move")
    {
        general_tree<LifecycleCounter> gt(LifecycleCounter{});
        CHECK_EQ(LifecycleCounter::move_constructor_calls, 1);
        CHECK_EQ(LifecycleCounter::copy_constructor_calls, 0);
    }
}

TEST_CASE_FIXTURE(LifecycleCounterFixture, "root value construction (emplacement)")
{
    SUBCASE("provided vale is stored in root with no copies")
    {
        general_tree<LifecycleCounter> gt("string123", 100);
        CHECK_EQ(gt.root().data().get_string(), "string123");
        CHECK_EQ(gt.root().data().get_int(), 100);
        CHECK_EQ(LifecycleCounter::move_constructor_calls, 0);
        CHECK_EQ(LifecycleCounter::copy_constructor_calls, 0);
    }
}

TEST_CASE_FIXTURE(LifecycleCounterFixture, "move constructor")
{
    SUBCASE("do not trigger any T's constructor")
    {
        general_tree<LifecycleCounter> gt = seed_tree(5);
        auto gt2 = std::move(gt);
        CHECK_EQ(LifecycleCounter::move_constructor_calls, 0);
        CHECK_EQ(LifecycleCounter::copy_constructor_calls, 0);
        CHECK_EQ(LifecycleCounter::parameterized_constructor_calls, 0);
    }

    SUBCASE("other tree remains empty after move")
    {
        general_tree<LifecycleCounter> gt = seed_tree(2);
        auto gt2 = std::move(gt);
        REQUIRE(gt.empty());
    }

    SUBCASE("left tree contains all the nodes of the right tree")
    {
        // TODO: create an array based on an iteration type.
    }

    SUBCASE("no error if the other tree is empty")
    {
        general_tree<int> empty;
        general_tree<LifecycleCounter> gt = seed_tree(5);
        REQUIRE_NOTHROW(auto gt2 = std::move(gt));
    }

    SUBCASE("moved-from tree can be reused normally afterwards")
    {
        general_tree<LifecycleCounter> moved_from = seed_tree(3);
        auto gt = std::move(moved_from);

        moved_from.emplace_root("string", 0);
        moved_from.emplace_left_child(moved_from.root(), "string1", 1);
        moved_from.emplace_right_sibling(moved_from.root().left_child(), "string2", 2);
        moved_from.clear();
        // new nodes destroyed successfully
        REQUIRE_EQ(LifecycleCounter::destructor_calls, 3);
    }   
}