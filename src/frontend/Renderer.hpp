#pragma once

#include <cstdint>
#include <expected>
#include <memory>
#include <string>

#include "frontend/Texture.hpp"
#include "frontend/Window.hpp"

// NOLINTNEXTLINE(readability-identifier-naming)
struct SDL_Renderer;

namespace Frontend
{
struct SdlRendererDeleter
{
    void operator()(SDL_Renderer* renderer) const noexcept;
};

enum class PresentationMode : uint8_t
{
    Disabled,
    Stretch,
    Letterbox,
    Overscan,
    IntegerScale
};

struct RendererConfig
{
    bool Vsync{true};
};

class Renderer
{
public:
    [[nodiscard]] static std::expected<Renderer, const char*> Create(const Window& window,
                                                                     const RendererConfig& config = {}) noexcept;

    ~Renderer() = default;

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer(Renderer&&) noexcept = default;
    Renderer& operator=(Renderer&&) noexcept = default;

    void Clear(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255) const noexcept;
    void Present() const noexcept;

    void Draw(const Texture& texture) const noexcept;

    void SetVSync(bool enabled) const noexcept;
    void SetLogicalPresentation(int width, int height, PresentationMode mode) const noexcept;

    [[nodiscard]] std::expected<Texture, const char*> CreateStreamingTexture(int width, int height) const noexcept;

    [[nodiscard]] SDL_Renderer* GetNativeRenderer() const noexcept;

private:
    explicit Renderer(SDL_Renderer* renderer) noexcept;

    std::unique_ptr<SDL_Renderer, SdlRendererDeleter> m_renderer;
};
} // namespace Frontend
