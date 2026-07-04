#pragma once

#include <array>
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

private:
    static constexpr std::array<Types::Pixel, 4> kPalette = {0xFFE0F8D0, 0xFF88C070, 0xFF346856, 0xFF081820};

    std::array<Types::Pixel, Constants::kScreenPixels> m_framebuffer{};
};
} // namespace Core::Video
