#include "Renderer.hpp"

#include <SDL3/SDL.h>

#include <cstdint>
#include <expected>
#include <string_view>
#include <utility>

#include "common/Logger.hpp"
#include "frontend/Texture.hpp"
#include "frontend/Window.hpp"

namespace Frontend
{
namespace
{
constexpr std::string_view ToString(PresentationMode mode) noexcept
{
    switch (mode)
    {
        case PresentationMode::Disabled:
            return "Disabled";
        case PresentationMode::Stretch:
            return "Stretch";
        case PresentationMode::Letterbox:
            return "Letterbox";
        case PresentationMode::Overscan:
            return "Overscan";
        case PresentationMode::IntegerScale:
            return "IntegerScale";
        default:
            std::unreachable();
    }
}
} // namespace

void SdlRendererDeleter::operator()(SDL_Renderer* renderer) const noexcept
{
    Common::LogInfo("[Renderer] Destroying renderer");
    SDL_DestroyRenderer(renderer);
}

std::expected<Renderer, const char*> Renderer::Create(const Window& window, const RendererConfig& config) noexcept
{
    Common::LogInfo("[Renderer] Initializing renderer");

    SDL_Renderer* rawRenderer = SDL_CreateRenderer(window.GetNativeWindow(), nullptr);

    if (!rawRenderer)
    {
        Common::LogError("[Renderer] SDL_CreateRenderer failed: '{}'", SDL_GetError());
        return std::unexpected(SDL_GetError());
    }

    Common::LogInfo("[Renderer] Enabling VSync");
    SDL_SetRenderVSync(rawRenderer, config.Vsync ? 1 : 0);

    if (const char* rendererName = SDL_GetRendererName(rawRenderer))
    {
        Common::LogInfo("[Renderer] Selected Graphics Backend: '{}'", rendererName);
    }

    return Renderer{rawRenderer};
}

Renderer::Renderer(SDL_Renderer* renderer) noexcept : m_renderer{renderer} {}

void Renderer::Clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a) const noexcept
{
    SDL_SetRenderDrawColor(m_renderer.get(), r, g, b, a);
    SDL_RenderClear(m_renderer.get());
}

void Renderer::Present() const noexcept
{
    SDL_RenderPresent(m_renderer.get());
}

void Renderer::Draw(const Texture& texture) const noexcept
{
    SDL_RenderTexture(m_renderer.get(), texture.GetNativeTexture(), nullptr, nullptr);
}

void Renderer::SetLogicalPresentation(int width, int height, PresentationMode mode) const noexcept
{
    Common::LogInfo("[Renderer] Set logical presentation: {}x{} (Mode: '{}')", width, height, ToString(mode));

    SDL_RendererLogicalPresentation sdlMode{};

    switch (mode)
    {
        case PresentationMode::Disabled:
            sdlMode = SDL_LOGICAL_PRESENTATION_DISABLED;
            break;
        case PresentationMode::Stretch:
            sdlMode = SDL_LOGICAL_PRESENTATION_STRETCH;
            break;
        case PresentationMode::Letterbox:
            sdlMode = SDL_LOGICAL_PRESENTATION_LETTERBOX;
            break;
        case PresentationMode::Overscan:
            sdlMode = SDL_LOGICAL_PRESENTATION_OVERSCAN;
            break;
        case PresentationMode::IntegerScale:
            sdlMode = SDL_LOGICAL_PRESENTATION_INTEGER_SCALE;
            break;
        default:
            std::unreachable();
    }

    SDL_SetRenderLogicalPresentation(m_renderer.get(), width, height, sdlMode);
}

std::expected<Texture, const char*> Renderer::CreateStreamingTexture(int width, int height) const noexcept
{
    Common::LogInfo("[Renderer] Creating texture: {}x{} (SDL_PIXELFORMAT_XRGB8888)", width, height);

    SDL_Texture* rawTexture =
        SDL_CreateTexture(m_renderer.get(), SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);

    if (!rawTexture)
    {
        Common::LogError("[Renderer] SDL_CreateTexture failed: '{}'", SDL_GetError());
        return std::unexpected(SDL_GetError());
    }

    return Texture{rawTexture, width, height};
}

SDL_Renderer* Renderer::GetNativeRenderer() const noexcept
{
    return m_renderer.get();
}
} // namespace Frontend
