#pragma once

#include <cstdint>

namespace Core::Audio
{
class Apu
{
public:
    Apu() = default;
    ~Apu() = default;

    Apu(const Apu&) = delete;
    Apu& operator=(const Apu&) = delete;
    Apu(Apu&&) noexcept = default;
    Apu& operator=(Apu&&) noexcept = default;

    [[nodiscard]] uint8_t Read(uint16_t address) const noexcept { return 0xFF; }

    void Write(uint16_t address, uint8_t value) noexcept {}
};
} // namespace Core::Audio
