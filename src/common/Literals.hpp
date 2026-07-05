#pragma once

#include <cstddef>

namespace Common::Literals
{
[[nodiscard]] constexpr size_t operator""_KiB(unsigned long long int val) noexcept
{
    return static_cast<size_t>(val * 1024);
}
} // namespace Common::Literals
