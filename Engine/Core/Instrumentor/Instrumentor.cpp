#include "Instrumentor.hpp"

MCEngine::Instrumentor &MCEngine::Instrumentor::GetInstance()
{
    static Instrumentor instance;
    return instance;
}

void MCEngine::Instrumentor::BeginSession(const std::string &filepath)
{
    m_OutputStream.open(filepath);
    WriteHeader();
}

void MCEngine::Instrumentor::EndSession()
{
    WriteFooter();
    m_OutputStream.close();

    // Reset profile count for next session
    m_ProfileCount = 0;
}

void MCEngine::Instrumentor::WriteProfile(const ProfileResult &result)
{
    if (m_ProfileCount++ > 0)
        m_OutputStream << ",";

    std::string name = result.Name;
    std::replace(name.begin(), name.end(), '"', '\'');

    m_OutputStream << "{";
    m_OutputStream << "\"cat\":\"function\",";
    m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
    m_OutputStream << "\"name\":\"" << name << "\",";
    m_OutputStream << "\"ph\":\"X\",";
    m_OutputStream << "\"pid\":0,";
    m_OutputStream << "\"tid\":" << result.ThreadID << ",";
    m_OutputStream << "\"ts\":" << result.Start;
    m_OutputStream << "}";

    m_OutputStream.flush();
}

void MCEngine::Instrumentor::WriteHeader()
{
    m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
    m_OutputStream.flush();
}

void MCEngine::Instrumentor::WriteFooter()
{
    m_OutputStream << "]}";
    m_OutputStream.flush();
}
