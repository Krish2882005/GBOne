#include "Window.hpp"

#include <SDL3/SDL.h>

#include <expected>

#include "common/Logger.hpp"

namespace Frontend
{
void SdlWindowDeleter::operator()(SDL_Window* window) const noexcept
{
    Common::LogInfo("[Window] Destroying window");
    SDL_DestroyWindow(window);
}

std::expected<Window, const char*> Window::Create(const WindowConfig& config) noexcept
{
    Common::LogInfo("[Window] Creating window '{}' ({}x{})", config.Title, config.Width, config.Height);

    SDL_WindowFlags flags = 0;

    if (config.Resizable)
    {
        flags |= SDL_WINDOW_RESIZABLE;
    }

    SDL_Window* rawWindow = SDL_CreateWindow(config.Title, config.Width, config.Height, flags);

    if (!rawWindow)
    {
        Common::LogError("[Window] SDL_CreateWindow failed: '{}'", SDL_GetError());
        return std::unexpected(SDL_GetError());
    }

    return Window{rawWindow};
}

Window::Window(SDL_Window* window) noexcept : m_window{window} {}

SDL_Window* Window::GetNativeWindow() const noexcept
{
    return m_window.get();
}
} // namespace Frontend
