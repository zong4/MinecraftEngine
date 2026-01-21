#pragma once

#include <Platform.hpp>

namespace Engine
{

struct Console
{
    bool ScrollToBottom = true;
    char InputBuf[256]{};
    std::vector<std::string> Items;
    std::vector<std::string> History;

public:
    void AddLog(const std::string &log)
    {
        Items.push_back(log);
        ScrollToBottom = true;
    }
};
extern Console g_Console;

using CommandFunction = std::function<void(const std::vector<std::string> &args)>;

class CommandManager
{
public:
    static CommandManager &GetInstance();

public:
    static std::vector<std::string> Tokenize(const std::string &str);
    void Register(const std::string &name, CommandFunction function) { m_Commands[name] = function; }
    void Execute(const std::string &input);

private:
    std::unordered_map<std::string, CommandFunction> m_Commands;

private:
    CommandManager() = default;
    ~CommandManager() = default;
};

} // namespace Engine