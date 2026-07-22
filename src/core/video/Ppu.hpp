#pragma once

#include <array>
#include <cstdint>
#include <span>

#include "core/Constants.hpp"
#include "core/Types.hpp"

namespace Core::Video
{
class Ppu
{
public:
    Ppu() noexcept;
    ~Ppu() = default;

    Ppu(const Ppu&) = delete;
    Ppu& operator=(const Ppu&) = delete;
    Ppu(Ppu&&) noexcept = default;
    Ppu& operator=(Ppu&&) noexcept = default;

    [[nodiscard]] std::span<const Types::Pixel, Constants::kScreenPixels> GetFramebufferView() const noexcept;

    [[nodiscard]] uint8_t ReadVram(uint16_t address) const noexcept;
    void WriteVram(uint16_t address, uint8_t value) noexcept;

private:
    static constexpr std::array<Types::Pixel, 4> kPalette = {0xFFE0F8D0, 0xFF88C070, 0xFF346856, 0xFF081820};

    std::array<Types::Pixel, Constants::kScreenPixels> m_framebuffer{};

    std::array<uint8_t, Constants::kVramSize> m_vram{};
};
} // namespace Core::Video
