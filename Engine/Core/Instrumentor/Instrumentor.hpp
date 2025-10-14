#pragma once

#include "pch.hpp"

namespace MCEngine
{

struct ProfileResult
{
    std::string Name;
    long long Start, End;
    uint32_t ThreadID;
};

class Instrumentor
{
public:
    static Instrumentor &GetInstance();

public:
    void BeginSession(const std::string &filepath);
    void WriteProfile(const ProfileResult &result);
    void EndSession();

private:
    std::ofstream m_OutputStream = {};
    int m_ProfileCount = 0;

private:
    Instrumentor() = default;
    ~Instrumentor() = default;

    void WriteHeader();
    void WriteFooter();
};

} // namespace MCEngine
