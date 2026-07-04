#include "System.hpp"

#include <span>

#include "core/Constants.hpp"
#include "core/Types.hpp"

namespace Core
{
void System::StepFrame() noexcept {}

std::span<const Types::Pixel, Constants::kScreenPixels> System::GetFramebufferView() const noexcept
{
    return m_ppu.GetFramebufferView();
}
} // namespace Core
