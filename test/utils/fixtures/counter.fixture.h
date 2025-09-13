#pragma once
#include <iostream>
#include <utility>

class Counter
{
private:
    std::string m_string = "";
    int m_int = 0;

public:
    static inline unsigned copy_constructor_calls;
    static inline unsigned move_constructor_calls;
    static inline unsigned destructor_calls;
    static inline unsigned copy_assignment_calls;
    static inline unsigned move_assignment_calls;

    static void reset() noexcept
    {
        Counter::copy_constructor_calls = 0;
        Counter::move_constructor_calls = 0;
        Counter::destructor_calls = 0;
        Counter::copy_assignment_calls = 0;
        Counter::move_assignment_calls = 0;
    }

    Counter() = default;

    Counter(const std::string& string, int integer)
        : m_string(string), m_int(integer) {}

    Counter(const Counter& rhs)
        : m_string(rhs.m_string), m_int(rhs.m_int)
    {
        ++copy_constructor_calls;
    }

    Counter(Counter&& rhs) noexcept
        : m_string(std::move(rhs.m_string)), m_int(rhs.m_int)
    {
        ++move_constructor_calls;
    }

    Counter& operator=(const Counter& rhs)
    {
        if (this != &rhs)
        {
            m_string = rhs.m_string;
            m_int = rhs.m_int;
            ++copy_assignment_calls;
        }
        return *this;
    }

    Counter& operator=(Counter&& rhs) noexcept
    {
        if (this != &rhs)
        {
            m_string = std::move(rhs.m_string);
            m_int = rhs.m_int;
            ++move_assignment_calls;
        }
        return *this;
    }

    ~Counter()
    {
        ++destructor_calls;
    }

    [[nodiscard]] const std::string& get_string() const noexcept { return m_string; }
    [[nodiscard]] int get_int() const noexcept { return m_int; }
};


struct CounterFixture
{
	CounterFixture() { Counter::reset(); }
	~CounterFixture() = default;
};
