#include <SDL3/SDL_main.h>

#include "common/Logger.hpp"
#include "frontend/Application.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    const Frontend::ApplicationConfig appConfig{
        .Title = "GBOne v1.0.0", .WindowWidth = 1280, .WindowHeight = 1152, .Vsync = true};

    auto appResult = Frontend::Application::Create(appConfig);
    if (!appResult.has_value())
    {
        Common::LogError("Fatal: Application Initialization failed: '{}'", appResult.error());
        return -1;
    }

    (*appResult)->Run();

    return 0;
}
