#pragma once

#include <span>

#include "core/Constants.hpp"
#include "core/Types.hpp"
#include "core/video/Ppu.hpp"

namespace Core
{
class System
{
public:
    System() = default;
    ~System() = default;

    System(const System&) = delete;
    System& operator=(const System&) = delete;
    System(System&&) noexcept = default;
    System& operator=(System&&) noexcept = default;

    void StepFrame() noexcept;

    [[nodiscard]] std::span<const Types::Pixel, Constants::kScreenPixels> GetFramebufferView() const noexcept;

private:
    Video::Ppu m_ppu;
};
} // namespace Core
