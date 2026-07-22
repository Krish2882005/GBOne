#pragma once

#include <cstdint>

namespace Core::Peripheral
{
class Joypad
{
public:
    Joypad() = default;
    ~Joypad() = default;

    Joypad(const Joypad&) = delete;
    Joypad& operator=(const Joypad&) = delete;
    Joypad(Joypad&&) noexcept = default;
    Joypad& operator=(Joypad&&) noexcept = default;

    [[nodiscard]] uint8_t Read(uint16_t address) const noexcept { return 0xFF; }

    void Write(uint16_t address, uint8_t value) noexcept {}
};
} // namespace Core::Peripheral
