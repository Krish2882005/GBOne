#pragma once

#include <cstdlib>
#include <print>
#include <source_location>

namespace Common
{
inline void CoreAssert(bool condition,
                       const char* message,
                       const std::source_location location = std::source_location::current()) noexcept
{
    if (!condition)
    {
        std::println(stderr,
                     "Assertion failed at {}:{}:{}: '{}'",
                     location.file_name(),
                     location.line(),
                     location.column(),
                     message);
        std::abort();
    }
}
} // namespace Common
