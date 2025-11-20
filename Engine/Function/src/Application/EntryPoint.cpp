#include "Application.hpp"

int main()
{
    // Create Application
    std::unique_ptr<Engine::Application> app;
    {
        Engine::Instrumentor::GetInstance().BeginSession(std::string(PROJECT_ROOT) + "/Logs/EngineProfile-Init.json");
        Engine::Logger::Init(std::string(PROJECT_ROOT) + "/Logs");
        app = Engine::CreateApplication();
        Engine::Instrumentor::GetInstance().EndSession();
    }

    // Run Application
    {
        Engine::Instrumentor::GetInstance().BeginSession(std::string(PROJECT_ROOT) +
                                                         "/Logs/EngineProfile-Runtime.json");
        app->Run();
        Engine::Instrumentor::GetInstance().EndSession();
    }

    // Shutdown Application
    {
        Engine::Instrumentor::GetInstance().BeginSession(std::string(PROJECT_ROOT) +
                                                         "/Logs/EngineProfile-Shutdown.json");
        app.reset();
        Engine::Instrumentor::GetInstance().EndSession();
    }

    return 0;
}