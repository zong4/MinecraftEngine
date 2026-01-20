#pragma once

#include <Platform.hpp>

namespace Engine
{

struct Console
{
    char InputBuf[256];
    std::vector<std::string> Items;
    std::vector<std::string> History;
    int HistoryPos = -1;
    bool AutoScroll = true;
};
extern Console g_Console;

using CommandFunction = std::function<void(const std::vector<std::string> &args)>;

class CommandManager
{
public:
    static CommandManager &GetInstance();

public:
    static std::vector<std::string> Tokenize(const std::string &str);
    static void Log(const std::string &text) { g_Console.Items.push_back(text); }
    void Register(const std::string &name, CommandFunction function) { m_Commands[name] = function; }
    void Execute(const std::string &input);

private:
    std::unordered_map<std::string, CommandFunction> m_Commands;
};

} // namespace Engine