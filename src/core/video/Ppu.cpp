#include "Ppu.hpp"

#include <cstdint>
#include <span>

#include "core/Constants.hpp"
#include "core/Types.hpp"

namespace Core::Video
{
Ppu::Ppu() noexcept
{
    m_framebuffer.fill(kPalette.front());
    m_vram.fill(0x00);
}

std::span<const Types::Pixel, Constants::kScreenPixels> Ppu::GetFramebufferView() const noexcept
{
    return m_framebuffer;
}

uint8_t Ppu::ReadVram(uint16_t address) const noexcept
{
    if (address < Constants::kVramBaseAddress || address >= (Constants::kVramBaseAddress + Constants::kVramSize))
    {
        return 0xFF;
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
    return m_vram[address - Constants::kVramBaseAddress];
}

void Ppu::WriteVram(uint16_t address, uint8_t value) noexcept
{
    if (address < Constants::kVramBaseAddress || address >= (Constants::kVramBaseAddress + Constants::kVramSize))
    {
        return;
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
    m_vram[address - Constants::kVramBaseAddress] = value;
}
} // namespace Core::Video
