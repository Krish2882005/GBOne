#include "Application.hpp"

#include <SDL3/SDL.h>

#include <expected>
#include <memory>
#include <utility>

#include "common/Logger.hpp"
#include "core/Constants.hpp"
#include "frontend/Renderer.hpp"
#include "frontend/Texture.hpp"
#include "frontend/Window.hpp"

namespace Frontend
{
std::expected<std::unique_ptr<Application>, const char*> Application::Create(const ApplicationConfig& config) noexcept
{
    Common::LogInfo("[Application] Booting GBOne Emulator v1.0.0");

    Common::LogInfo(
        "[Application] SDL Compiled Version: {}.{}.{}", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);

    const int runtimeVersion = SDL_GetVersion();
    Common::LogInfo("[Application] SDL Runtime Version: {}.{}.{}",
                    SDL_VERSIONNUM_MAJOR(runtimeVersion),
                    SDL_VERSIONNUM_MINOR(runtimeVersion),
                    SDL_VERSIONNUM_MICRO(runtimeVersion));

    Common::LogInfo("[Application] Initializing subsystems");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        Common::LogError("[Application] Initialization failed: '{}'", SDL_GetError());
        return std::unexpected(SDL_GetError());
    }

    const WindowConfig winConfig{.Title = config.Title, .Width = config.WindowWidth, .Height = config.WindowHeight};

    auto windowResult = Window::Create(winConfig);
    if (!windowResult.has_value())
    {
        Common::LogError("[Application] Window creation failed: '{}'", windowResult.error());
        SDL_Quit();
        return std::unexpected(windowResult.error());
    }

    const RendererConfig renConfig{.Vsync = config.Vsync};

    auto rendererResult = Renderer::Create(*windowResult, renConfig);
    if (!rendererResult.has_value())
    {
        Common::LogError("[Application] Renderer creation failed: '{}'", rendererResult.error());
        SDL_Quit();
        return std::unexpected(rendererResult.error());
    }

    rendererResult->SetLogicalPresentation(
        Core::Constants::kScreenWidth, Core::Constants::kScreenHeight, Frontend::PresentationMode::Letterbox);

    auto textureResult =
        rendererResult->CreateStreamingTexture(Core::Constants::kScreenWidth, Core::Constants::kScreenHeight);
    if (!textureResult.has_value())
    {
        Common::LogError("[Application] Texture creation failed: '{}'", textureResult.error());
        SDL_Quit();
        return std::unexpected(textureResult.error());
    }

    textureResult->SetScaleMode(TextureScaleMode::PixelArt);

    Common::LogInfo("[Application] Initialization complete");

    // NOLINTBEGIN(bugprone-exception-escape, bugprone-unhandled-exception-at-new)
    return std::unique_ptr<Application>(
        new Application(std::move(*windowResult), std::move(*rendererResult), std::move(*textureResult)));
    // NOLINTEND(bugprone-exception-escape, bugprone-unhandled-exception-at-new)
}

Application::Application(Window window, Renderer renderer, Texture displayTexture) noexcept
    : m_window{std::move(window)}, m_renderer{std::move(renderer)}, m_displayTexture{std::move(displayTexture)}
{
}

Application::~Application()
{
    Common::LogInfo("[Application] Shutting down subsystems");
    SDL_Quit();
}

void Application::Quit() noexcept
{
    Common::LogInfo("[Application] Quit requested");
    m_isRunning = false;
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
bool Application::ProcessEvents() noexcept
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            Common::LogInfo("[Application] Quit event received");
            return false;
        }
    }

    return true;
}

void Application::Run() noexcept
{
    Common::LogInfo("[Application] Entering main loop");
    m_isRunning = true;

    while (m_isRunning)
    {
        if (!ProcessEvents())
        {
            break;
        }

        m_core.StepFrame();

        m_renderer.Clear();
        m_displayTexture.Update(m_core.GetFramebufferView());
        m_renderer.Draw(m_displayTexture);
        m_renderer.Present();
    }

    Common::LogInfo("[Application] Exiting main loop");
}
} // namespace Frontend
