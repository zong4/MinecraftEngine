#include "Logger.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

std::shared_ptr<spdlog::logger> Engine::Logger::s_EngineLogger = nullptr;
std::shared_ptr<spdlog::logger> Engine::Logger::s_EditorLogger = nullptr;

void Engine::Logger::Init(const std::string &dirPath)
{
    if (s_EngineLogger && s_EditorLogger)
        return;

    s_EngineLogger = CreateLogger(dirPath, "Engine");
    LOG_ENGINE_INFO("Engine logger initialized");

    s_EditorLogger = CreateLogger(dirPath, "Editor");
    LOG_EDITOR_INFO("Editor logger initialized");
}

void Engine::Logger::LogEngineTrace(const std::string &message) { s_EngineLogger->trace(message); }
void Engine::Logger::LogEngineInfo(const std::string &message) { s_EngineLogger->info(message); }
void Engine::Logger::LogEngineWarn(const std::string &message) { s_EngineLogger->warn("Warning: " + message); }
void Engine::Logger::LogEngineError(const std::string &message, bool assertFail)
{
    s_EngineLogger->error("Error: " + message);
    if (assertFail)
        assert(false);
}

void Engine::Logger::LogEditorTrace(const std::string &message) { s_EditorLogger->trace(message); }
void Engine::Logger::LogEditorInfo(const std::string &message) { s_EditorLogger->info(message); }
void Engine::Logger::LogEditorWarn(const std::string &message) { s_EditorLogger->warn("Warning: " + message); }
void Engine::Logger::LogEditorError(const std::string &message, bool assertFail)
{
    s_EditorLogger->error("Error: " + message);
    assert(false);
}

std::shared_ptr<spdlog::logger> Engine::Logger::CreateLogger(const std::string &dirPath, const std::string &name)
{
    // Create console sink and file sink
    auto &&console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto &&file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(dirPath + "/" + name + ".log", true);
    std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
    std::shared_ptr<spdlog::logger> logger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());

    // Set logger pattern and level
    spdlog::register_logger(logger);
    logger->set_pattern("%^[%T] [%n] [thread %t] %v%$");
    logger->set_level(spdlog::level::trace);
    return logger;
}