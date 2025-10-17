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

TEST_CASE_FIXTURE(LifecycleCounterFixture, "copy constructor")
{
    SUBCASE("no error if other tree is empty")
    {
        general_tree<int> empty_tree;
        general_tree<int> gt(empty_tree);
        REQUIRE(gt.empty());
    }

    SUBCASE("copy unbalanced tree")
    {
        general_tree<int> gt(0);
        auto node = gt.root();
        for (unsigned i = 0; i < 25; i++)
            node = gt.insert_left_child(node, i + 1);

        general_tree<int> new_tree(gt);
        REQUIRE_EQ(new_tree, gt);
    }

    SUBCASE("copy tree with only one node")
    {
        general_tree<int> gt(1);
        general_tree<int> new_tree(gt);
        REQUIRE_EQ(new_tree.root().data(), 1);
    }

    SUBCASE("trees are equal after operation")
    {
        general_tree<LifecycleCounter> gt = seed_tree(11);
        general_tree<LifecycleCounter> new_tree(gt);
        REQUIRE_EQ(gt, new_tree);
    }

    SUBCASE("other tree is not modified")
    {
        general_tree<LifecycleCounter> gt = seed_tree(5);
        auto backup = gt;

        general_tree<LifecycleCounter> new_tree(gt);

        REQUIRE_EQ(backup, gt);
    }

    SUBCASE("copy as many nodes as the original tree contains")
    {
        std::size_t org_tree_size = 4;
        general_tree<LifecycleCounter> org_tree = seed_tree(org_tree_size);
        general_tree<LifecycleCounter> new_tree(org_tree);
        REQUIRE_EQ(LifecycleCounter::copy_constructor_calls, org_tree_size);
    }

    SUBCASE("all the copies nodes are destroyed successfully when clear is called")
    {
        general_tree<LifecycleCounter> gt = seed_tree(7);
        general_tree<LifecycleCounter> new_tree(gt);
        new_tree.clear();
        REQUIRE_EQ(LifecycleCounter::destructor_calls, 7);
    }

    SUBCASE("inserting values after operation is safe")
    {
        std::size_t org_tree_size = 3;
        general_tree<LifecycleCounter> gt = seed_tree(org_tree_size);
        general_tree<LifecycleCounter> new_tree(gt);

        new_tree.emplace_left_child(new_tree.root(), "string99", 99);
        new_tree.emplace_left_child(new_tree.root().left_child(), "string100", 100);

        new_tree.clear();
        REQUIRE_EQ(LifecycleCounter::destructor_calls, org_tree_size + 2);
    }
}

TEST_CASE_FIXTURE(LifecycleCounterFixture, "root value construction")
{
    SUBCASE("create one copy of the provided value and store it in root")
    {
        LifecycleCounter value("string101", 101);
        general_tree<LifecycleCounter> gt(value);
        CHECK_EQ(gt.root().data().get_int(), value.get_int());
        CHECK_EQ(gt.root().data().get_string(), value.get_string());
        CHECK_EQ(LifecycleCounter::move_constructor_calls, 0);
        CHECK_EQ(LifecycleCounter::copy_constructor_calls, 1);
        CHECK_EQ(LifecycleCounter::parameterized_constructor_calls, 1);
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
        general_tree<LifecycleCounter> gt = seed_tree(5);
        auto backup = gt;

        auto gt2 = std::move(gt);
        REQUIRE_EQ(backup, gt2);
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
