#pragma once

#include <expected>
#include <memory>
#include <string>

// NOLINTNEXTLINE(readability-identifier-naming)
struct SDL_Window;

namespace Frontend
{
struct SdlWindowDeleter
{
    void operator()(SDL_Window* window) const noexcept;
};

struct WindowConfig
{
    const char* Title{"GBOne"};
    int Width{1920};
    int Height{1080};
    bool Resizable{true};
};

class Window
{
public:
    [[nodiscard]] static std::expected<Window, const char*> Create(const WindowConfig& config = {}) noexcept;

    ~Window() = default;

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) noexcept = default;
    Window& operator=(Window&&) noexcept = default;

    [[nodiscard]] SDL_Window* GetNativeWindow() const noexcept;

private:
    explicit Window(SDL_Window* window) noexcept;

    std::unique_ptr<SDL_Window, SdlWindowDeleter> m_window;
};
} // namespace Frontend
