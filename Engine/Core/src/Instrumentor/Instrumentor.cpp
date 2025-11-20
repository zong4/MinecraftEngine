#include "Instrumentor.hpp"

Engine::Instrumentor &Engine::Instrumentor::GetInstance()
{
    static Instrumentor instance;
    return instance;
}

void Engine::Instrumentor::BeginSession(const std::string &filepath)
{
    m_OutputStream.open(filepath);
    WriteHeader();
}

void Engine::Instrumentor::EndSession()
{
    WriteFooter();
    m_OutputStream.close();

    // Reset profile count for next session
    m_ProfileCount = 0;
}

void Engine::Instrumentor::WriteProfile(const ProfileResult &result)
{
    if (m_ProfileCount++ > 0)
        m_OutputStream << ",";

    // Sanitize name by replacing double quotes with single quotes
    std::string name = result.Name;
    std::replace(name.begin(), name.end(), '"', '\'');

    // Write profile entry in JSON format
    m_OutputStream << "{";
    m_OutputStream << "\"cat\":\"function\",";
    m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
    m_OutputStream << "\"name\":\"" << name << "\",";
    m_OutputStream << "\"ph\":\"X\",";
    m_OutputStream << "\"pid\":0,";
    m_OutputStream << "\"tid\":" << result.ThreadID << ",";
    m_OutputStream << "\"ts\":" << result.Start;
    m_OutputStream << "}";

    // Flush the output stream
    m_OutputStream.flush();
}

void Engine::Instrumentor::WriteHeader()
{
    m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
    m_OutputStream.flush();
}

void Engine::Instrumentor::WriteFooter()
{
    m_OutputStream << "]}";
    m_OutputStream.flush();
}
