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

    auto &&it = m_Commands.find(tokens[0]);
    if (it != m_Commands.end())
    {
        tokens.erase(tokens.begin());
        it->second(tokens);
    }
    else
    {
        Log("Unknown command: " + tokens[0]);
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