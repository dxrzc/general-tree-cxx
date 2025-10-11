#pragma once
#include "general-tree.h"
#include <cassert>
#include <string>
#include <utility>

class LifecycleCounter
{
private:
    std::string m_string = "";
    int m_int = 0;

public:
    static inline unsigned parameterized_constructor_calls;
    static inline unsigned copy_constructor_calls;
    static inline unsigned move_constructor_calls;
    static inline unsigned destructor_calls;
    static inline unsigned copy_assignment_calls;
    static inline unsigned move_assignment_calls;

    static void reset() noexcept
    {
        LifecycleCounter::parameterized_constructor_calls = 0;
        LifecycleCounter::copy_constructor_calls = 0;
        LifecycleCounter::move_constructor_calls = 0;
        LifecycleCounter::destructor_calls = 0;
        LifecycleCounter::copy_assignment_calls = 0;
        LifecycleCounter::move_assignment_calls = 0;
    }

    LifecycleCounter() = default;

    LifecycleCounter(const std::string &string, int integer)
        : m_string(string),
          m_int(integer)
    {
        ++parameterized_constructor_calls;
    }

    LifecycleCounter(const LifecycleCounter &rhs)
        : m_string(rhs.m_string),
          m_int(rhs.m_int)
    {
        ++copy_constructor_calls;
    }

    LifecycleCounter(LifecycleCounter &&rhs) noexcept
        : m_string(std::move(rhs.m_string)),
          m_int(rhs.m_int)
    {
        ++move_constructor_calls;
    }

    LifecycleCounter &operator=(const LifecycleCounter &rhs)
    {
        if (this != &rhs)
        {
            m_string = rhs.m_string;
            m_int = rhs.m_int;
            ++copy_assignment_calls;
        }
        return *this;
    }

    LifecycleCounter &operator=(LifecycleCounter &&rhs) noexcept
    {
        if (this != &rhs)
        {
            m_string = std::move(rhs.m_string);
            m_int = rhs.m_int;
            ++move_assignment_calls;
        }
        return *this;
    }

    bool operator==(const LifecycleCounter& rhs) const noexcept
    {
        return m_int == rhs.m_int && m_string == rhs.m_string;
    }

    ~LifecycleCounter()
    {
        ++destructor_calls;
    }

    [[nodiscard]] const std::string &get_string() const noexcept
    {
        return m_string;
    }

    [[nodiscard]] int get_int() const noexcept
    {
        return m_int;
    }
};

struct LifecycleCounterFixture
{
    LifecycleCounterFixture()
    {
        LifecycleCounter::reset();
    }
    ~LifecycleCounterFixture() = default;
};
