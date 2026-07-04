#pragma once

#include <bit>
#include <concepts>
#include <cstdint>

namespace Common::Bitwise
{
template <std::integral T>
[[nodiscard]] constexpr T SwapBytes(T value) noexcept
{
    return std::byteswap(value);
}

template <std::integral T>
[[nodiscard]] constexpr bool TestBit(T value, uint8_t bit) noexcept
{
    return (value & (static_cast<T>(1) << bit)) != 0;
}

template <std::integral T>
[[nodiscard]] constexpr T SetBit(T value, uint8_t bit) noexcept
{
    return value | (static_cast<T>(1) << bit);
}

template <std::integral T>
[[nodiscard]] constexpr T ClearBit(T value, uint8_t bit) noexcept
{
    return value & ~(static_cast<T>(1) << bit);
}

template <std::unsigned_integral T>
[[nodiscard]] constexpr T RotateLeft(T value, int amount = 1) noexcept
{
    return std::rotl(value, amount);
}

template <std::unsigned_integral T>
[[nodiscard]] constexpr T RotateRight(T value, int amount = 1) noexcept
{
    return std::rotr(value, amount);
}
} // namespace Common::Bitwise
