#include "CommandManager.hpp"

Engine::Console Engine::g_Console = {};

Engine::CommandManager &Engine::CommandManager::GetInstance()
{
    static CommandManager instance;
    return instance;
}

void Engine::CommandManager::Execute(const std::string &input)
{
    PROFILE_FUNCTION();

    auto &&tokens = Tokenize(input);
    if (tokens.empty())
        return;
    g_Console.AddLog("> " + input);

    auto &&it = m_Commands.find(tokens[0]);
    if (it != m_Commands.end())
    {
        tokens.erase(tokens.begin());
        it->second(tokens);
        g_Console.History.push_back(input);
    }
    else
    {
        g_Console.AddLog("Unknown command: " + tokens[0]);
    }
}

std::vector<std::string> Engine::CommandManager::Tokenize(const std::string &str)
{
    PROFILE_FUNCTION();

    std::vector<std::string> tokens;
    std::istringstream stream(str);
    std::string token;
    while (stream >> token)
        tokens.push_back(token);
    return tokens;
}