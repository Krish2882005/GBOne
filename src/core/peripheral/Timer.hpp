#pragma once

#include <cstdint>

namespace Core::Peripheral
{
class Timer
{
public:
    Timer() = default;
    ~Timer() = default;

    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;
    Timer(Timer&&) noexcept = default;
    Timer& operator=(Timer&&) noexcept = default;

    [[nodiscard]] uint8_t Read(uint16_t address) const noexcept { return 0xFF; }

    void Write(uint16_t address, uint8_t value) noexcept {}
};
} // namespace Core::Peripheral
