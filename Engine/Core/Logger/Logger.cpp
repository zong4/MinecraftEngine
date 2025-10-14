#include "Logger.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

std::shared_ptr<spdlog::logger> MCEngine::Logger::s_EngineLogger = nullptr;
std::shared_ptr<spdlog::logger> MCEngine::Logger::s_EditorLogger = nullptr;

void MCEngine::Logger::Init(const std::string &dirPath)
{
    if (s_EngineLogger != nullptr && s_EditorLogger != nullptr)
        return;

    s_EngineLogger = CreateLogger(dirPath, "Engine");
    LOG_ENGINE_INFO("Engine logger initialized");

    s_EditorLogger = CreateLogger(dirPath, "Editor");
    LOG_EDITOR_INFO("Editor logger initialized");
}

void MCEngine::Logger::LogEngineTrace(const std::string &message) { s_EngineLogger->trace(message); }
void MCEngine::Logger::LogEngineInfo(const std::string &message) { s_EngineLogger->info(message); }
void MCEngine::Logger::LogEngineWarn(const std::string &message) { s_EngineLogger->warn("Warning: " + message); }
void MCEngine::Logger::LogEngineError(const std::string &message)
{
    s_EngineLogger->error(message);
    assert(false);
}

void MCEngine::Logger::LogEditorTrace(const std::string &message) { s_EditorLogger->trace(message); }
void MCEngine::Logger::LogEditorInfo(const std::string &message) { s_EditorLogger->info(message); }
void MCEngine::Logger::LogEditorWarn(const std::string &message) { s_EditorLogger->warn("Warning: " + message); }
void MCEngine::Logger::LogEditorError(const std::string &message)
{
    s_EditorLogger->error(message);
    assert(false);
}

std::shared_ptr<spdlog::logger> MCEngine::Logger::CreateLogger(const std::string &dirPath, const std::string &name)
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