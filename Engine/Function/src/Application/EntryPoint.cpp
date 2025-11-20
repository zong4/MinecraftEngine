#include "Application.hpp"

int main()
{
    // Create Application
    std::unique_ptr<MCEngine::Application> app;
    {
        MCEngine::Instrumentor::GetInstance().BeginSession(std::string(PROJECT_ROOT) + "/Logs/EngineProfile-Init.json");
        MCEngine::Logger::Init(std::string(PROJECT_ROOT) + "/Logs");
        app = MCEngine::CreateApplication();
        MCEngine::Instrumentor::GetInstance().EndSession();
    }

    // Run Application
    {
        MCEngine::Instrumentor::GetInstance().BeginSession(std::string(PROJECT_ROOT) +
                                                           "/Logs/EngineProfile-Runtime.json");
        app->Run();
        MCEngine::Instrumentor::GetInstance().EndSession();
    }

    // Shutdown Application
    {
        MCEngine::Instrumentor::GetInstance().BeginSession(std::string(PROJECT_ROOT) +
                                                           "/Logs/EngineProfile-Shutdown.json");
        app.reset();
        MCEngine::Instrumentor::GetInstance().EndSession();
    }

    return 0;
}