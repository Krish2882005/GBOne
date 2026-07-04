#pragma once

#include <cstdint>
#include <memory>
#include <span>

// NOLINTNEXTLINE(readability-identifier-naming)
struct SDL_Texture;

namespace Frontend
{
struct SdlTextureDeleter
{
    void operator()(SDL_Texture* texture) const noexcept;
};

enum class TextureScaleMode : uint8_t
{
    Nearest,
    Linear,
    PixelArt
};

class Texture
{
public:
    ~Texture() = default;

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&&) noexcept = default;
    Texture& operator=(Texture&&) noexcept = default;

    void Update(std::span<const uint32_t> pixels) noexcept;

    void SetScaleMode(TextureScaleMode mode) noexcept;

    [[nodiscard]] SDL_Texture* GetNativeTexture() const noexcept;

private:
    friend class Renderer;
    Texture(SDL_Texture* texture, int width, int height) noexcept;

    std::unique_ptr<SDL_Texture, SdlTextureDeleter> m_texture;
    int m_width{0};
    int m_height{0};
};
} // namespace Frontend
