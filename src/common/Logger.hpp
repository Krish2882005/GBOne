#pragma once

#include <format>
#include <print>
#include <utility>

namespace Common
{
namespace Color
{
constexpr auto kReset = "\033[0m";
constexpr auto kGreen = "\033[32m";
constexpr auto kRed = "\033[31m";
} // namespace Color

template <typename... Args>
void LogInfo(std::format_string<Args...> fmt, Args&&... args) noexcept
{
    try
    {
        std::print(stdout, "{}", Color::kGreen);
        std::print(stdout, fmt, std::forward<Args>(args)...);
        std::println(stdout, "{}", Color::kReset);
    }
    catch (const std::exception& e)
    {
        (void)e;
    }
}

template <typename... Args>
void LogError(std::format_string<Args...> fmt, Args&&... args) noexcept
{
    try
    {
        std::print(stderr, "{}", Color::kRed);
        std::print(stderr, fmt, std::forward<Args>(args)...);
        std::println(stderr, "{}", Color::kReset);
    }
    catch (const std::exception& e)
    {
        (void)e;
    }
}
} // namespace Common
