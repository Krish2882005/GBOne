#include "Texture.hpp"

#include <SDL3/SDL.h>

#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>
#include <utility>

#include "common/Logger.hpp"

namespace Frontend
{
namespace
{
constexpr std::string_view ToString(TextureScaleMode mode) noexcept
{
    switch (mode)
    {
        case TextureScaleMode::Nearest:
            return "Nearest";
        case TextureScaleMode::Linear:
            return "Linear";
        case TextureScaleMode::PixelArt:
            return "PixelArt";
        default:
            std::unreachable();
    }
}
} // namespace

void SdlTextureDeleter::operator()(SDL_Texture* texture) const noexcept
{
    Common::LogInfo("[Texture] Destroying texture");
    SDL_DestroyTexture(texture);
}

Texture::Texture(SDL_Texture* texture, int width, int height) noexcept
    : m_texture{texture}, m_width{width}, m_height{height}
{
}

void Texture::Update(std::span<const uint32_t> pixels) noexcept
{
    const size_t expectedSize = static_cast<size_t>(m_width) * static_cast<size_t>(m_height);

    if (pixels.size() != expectedSize)
    {
        Common::LogError("[Texture] Texture update size mismatch: Expected {}, got {}", expectedSize, pixels.size());
        return;
    }

    SDL_UpdateTexture(m_texture.get(), nullptr, pixels.data(), static_cast<int>(m_width * sizeof(uint32_t)));
}

void Texture::SetScaleMode(TextureScaleMode mode) noexcept
{
    Common::LogInfo("[Texture] Set scale mode: '{}'", ToString(mode));

    SDL_ScaleMode sdlMode{};

    switch (mode)
    {
        case TextureScaleMode::Nearest:
            sdlMode = SDL_SCALEMODE_NEAREST;
            break;
        case TextureScaleMode::Linear:
            sdlMode = SDL_SCALEMODE_LINEAR;
            break;
        case TextureScaleMode::PixelArt:
            sdlMode = SDL_SCALEMODE_PIXELART;
            break;
        default:
            std::unreachable();
    }

    SDL_SetTextureScaleMode(m_texture.get(), sdlMode);
}

SDL_Texture* Texture::GetNativeTexture() const noexcept
{
    return m_texture.get();
}
} // namespace Frontend
