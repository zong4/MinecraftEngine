#include "Application.hpp"

int main()
{
    // Create Application
    std::unique_ptr<Engine::Application> app;
    {
        Engine::Instrumentor::GetInstance().BeginSession((Engine::GetLogsDirectory() / "Profile-Init.json").string());
        Engine::Logger::Init(Engine::GetLogsDirectory().string());
        app = Engine::CreateApplication();
        Engine::Instrumentor::GetInstance().EndSession();
    }

    // Run Application
    {
        Engine::Instrumentor::GetInstance().BeginSession(
            (Engine::GetLogsDirectory() / "Profile-Runtime.json").string());
        app->Run();
        Engine::Instrumentor::GetInstance().EndSession();
    }

    // Shutdown Application
    {
        Engine::Instrumentor::GetInstance().BeginSession(
            (Engine::GetLogsDirectory() / "Profile-Shutdown.json").string());
        app.reset();
        Engine::Instrumentor::GetInstance().EndSession();
    }

    return 0;
}