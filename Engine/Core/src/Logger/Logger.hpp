#pragma once

#include "pch.hpp"

namespace spdlog
{
class logger; // forward declaration
}

namespace MCEngine
{

class Logger
{
public:
    static void Init(const std::string &dirPath);

    static void LogEngineTrace(const std::string &message);
    static void LogEngineInfo(const std::string &message);
    static void LogEngineWarn(const std::string &message);
    static void LogEngineError(const std::string &message, bool assertFail);

    static void LogEditorTrace(const std::string &message);
    static void LogEditorInfo(const std::string &message);
    static void LogEditorWarn(const std::string &message);
    static void LogEditorError(const std::string &message, bool assertFail);

private:
    static std::shared_ptr<spdlog::logger> s_EngineLogger;
    static std::shared_ptr<spdlog::logger> s_EditorLogger;

private:
    static std::shared_ptr<spdlog::logger> CreateLogger(const std::string &dirPath, const std::string &name);
};

// Trace: Fine-grained log message for debugging
// Info: General log message to indicate program flow
// Warn: Meaningless issue happened, the program can continue running
// Error: Critical error happened, the program can recover
// Assert: Critical error happened, the program cannot recover

#ifdef DEBUG

#define LOG_ENGINE_TRACE(msg) MCEngine::Logger::LogEngineTrace(msg)
#define LOG_ENGINE_INFO(msg) MCEngine::Logger::LogEngineInfo(msg)
#define LOG_ENGINE_WARN(msg) MCEngine::Logger::LogEngineWarn(msg)
#define LOG_ENGINE_ERROR(msg) MCEngine::Logger::LogEngineError(msg, false);
#define LOG_ENGINE_ASSERT(msg) MCEngine::Logger::LogEngineError(msg, true);

#define LOG_EDITOR_TRACE(msg) MCEngine::Logger::LogEditorTrace(msg)
#define LOG_EDITOR_INFO(msg) MCEngine::Logger::LogEditorInfo(msg)
#define LOG_EDITOR_WARN(msg) MCEngine::Logger::LogEditorWarn(msg)
#define LOG_EDITOR_ERROR(msg) MCEngine::Logger::LogEditorError(msg, false);
#define LOG_EDITOR_ASSERT(msg) MCEngine::Logger::LogEditorError(msg, true);

#else

#define LOG_ENGINE_TRACE(msg)
#define LOG_ENGINE_INFO(msg)
#define LOG_ENGINE_WARN(msg)
#define LOG_ENGINE_ERROR(msg)
#define LOG_ENGINE_ASSERT(msg)

#define LOG_EDITOR_TRACE(msg)
#define LOG_EDITOR_INFO(msg)
#define LOG_EDITOR_WARN(msg)
#define LOG_EDITOR_ERROR(msg)
#define LOG_EDITOR_ASSERT(msg)

#endif

} // namespace MCEngine