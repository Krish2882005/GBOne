#pragma once

#include <cstdint>

namespace Core::Peripheral
{
class Serial
{
public:
    Serial() = default;
    ~Serial() = default;

    Serial(const Serial&) = delete;
    Serial& operator=(const Serial&) = delete;
    Serial(Serial&&) noexcept = default;
    Serial& operator=(Serial&&) noexcept = default;

    [[nodiscard]] uint8_t Read(uint16_t address) const noexcept { return 0xFF; }

    void Write(uint16_t address, uint8_t value) noexcept {}
};
} // namespace Core::Peripheral
