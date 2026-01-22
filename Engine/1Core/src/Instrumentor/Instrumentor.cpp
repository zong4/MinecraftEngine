#include "Instrumentor.hpp"

#include "Logger/Logger.hpp"

Engine::Instrumentor &Engine::Instrumentor::GetInstance()
{
    static Instrumentor instance;
    return instance;
}

void Engine::Instrumentor::BeginSession(const std::string &filepath)
{
    if (m_Active)
        return;

    // Open output file stream
    m_OutputStream.open(filepath);
    if (!m_OutputStream.is_open())
    {
        LOG_ENGINE_ERROR("Instrumentor could not open results file: " + filepath);
        return;
    }

    // Activate instrumentor
    m_Active = true;
    WriteHeader();

    // Start writer thread
    m_WriterThread = std::thread([this]() { WriterThreadFunc(); });
}

void Engine::Instrumentor::WriteProfile(const ProfileResult &result)
{
    if (!m_Active)
        return;

    // Push profile result to queue with lock
    {
        std::lock_guard<std::mutex> lock(m_QueueMutex);
        m_Queue.push_back(result);
    }
    m_CV.notify_one(); // wake writer thread
}

void Engine::Instrumentor::EndSession()
{
    m_Active = false;
    m_CV.notify_one(); // wake writer thread to finish

    // Wait for writer thread to join
    if (m_WriterThread.joinable())
        m_WriterThread.join();

    // Write footer and close file stream
    WriteFooter();
    if (m_OutputStream.is_open())
        m_OutputStream.close();
    m_ProfileCount = 0;
}

void Engine::Instrumentor::WriteHeader()
{
    m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
    m_OutputStream.flush();
}

void Engine::Instrumentor::WriterThreadFunc()
{
    while (m_Active || !m_Queue.empty())
    {
        // Wait for new profile results or session end
        std::unique_lock<std::mutex> lock(m_QueueMutex);
        m_CV.wait(lock, [this]() { return !m_Active || !m_Queue.empty(); });

        // Write all profile results in the queue
        while (!m_Queue.empty())
        {
            ProfileResult result = m_Queue.front();
            m_Queue.erase(m_Queue.begin());

            // Write profile result in JSON format
            if (m_ProfileCount++ > 0)
                m_OutputStream << ",";

            // Sanitize name
            std::string name = result.Name;
            std::replace(name.begin(), name.end(), '"', '\'');

            // Write JSON entry
            m_OutputStream << "{";
            m_OutputStream << "\"cat\":\"function\",";
            m_OutputStream << "\"dur\":" << (result.End - result.Start) << ",";
            m_OutputStream << "\"name\":\"" << name << "\",";
            m_OutputStream << "\"ph\":\"X\",";
            m_OutputStream << "\"pid\":0,";
            m_OutputStream << "\"tid\":" << result.ThreadID << ",";
            m_OutputStream << "\"ts\":" << result.Start;
            m_OutputStream << "}";

            // Flush output stream
            m_OutputStream.flush();
        }
    }
}

void Engine::Instrumentor::WriteFooter()
{
    m_OutputStream << "]}";
    m_OutputStream.flush();
}
