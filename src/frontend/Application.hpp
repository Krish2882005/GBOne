#pragma once

#include <expected>
#include <memory>
#include <string>

#include "core/System.hpp"
#include "frontend/Renderer.hpp"
#include "frontend/Texture.hpp"
#include "frontend/Window.hpp"

namespace Frontend
{
struct ApplicationConfig
{
    const char* Title{"GBOne"};
    int WindowWidth{1920};
    int WindowHeight{1080};
    bool Vsync{true};
};

class Application
{
public:
    [[nodiscard]] static std::expected<std::unique_ptr<Application>, const char*>
    Create(const ApplicationConfig& config = {}) noexcept;

    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) noexcept = default;
    Application& operator=(Application&&) noexcept = default;

    void Run() noexcept;
    void Quit() noexcept;

private:
    Application(Window window, Renderer renderer, Texture displayTexture) noexcept;

    [[nodiscard]] bool ProcessEvents() noexcept;

    Window m_window;
    Renderer m_renderer;
    Texture m_displayTexture;
    Core::System m_core;
    bool m_isRunning{false};
};
} // namespace Frontend
