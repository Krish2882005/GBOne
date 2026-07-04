#include "Ppu.hpp"

#include <span>

#include "core/Constants.hpp"
#include "core/Types.hpp"

namespace Core::Video
{
Ppu::Ppu() noexcept
{
    m_framebuffer.fill(kPalette.front());
}

std::span<const Types::Pixel, Constants::kScreenPixels> Ppu::GetFramebufferView() const noexcept
{
    return m_framebuffer;
}
} // namespace Core::Video
