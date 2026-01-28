#include "../ResourcesManager/ResourcesRoot.hpp"
#include "Application.hpp"

int main()
{
    // Create Application
    std::unique_ptr<Engine::Application> app;
    {
        Engine::Instrumentor::GetInstance().BeginSession(Engine::GetLogsRoot() / "Profile-Init.json");
        Engine::Logger::Init(Engine::GetLogsRoot());
        app = Engine::CreateApplication();
        Engine::Instrumentor::GetInstance().EndSession();
    }

    // Run Application
    {
        Engine::Instrumentor::GetInstance().BeginSession(Engine::GetLogsRoot() / "Profile-Runtime.json");
        app->Run();
        Engine::Instrumentor::GetInstance().EndSession();
    }

    // Shutdown Application
    {
        Engine::Instrumentor::GetInstance().BeginSession(Engine::GetLogsRoot() / "Profile-Shutdown.json");
        app.reset();
        Engine::Instrumentor::GetInstance().EndSession();
    }

    return 0;
}