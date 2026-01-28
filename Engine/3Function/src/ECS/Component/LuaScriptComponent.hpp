#pragma once

#include "../Entity/Entity.hpp"
#include <Platform.hpp>
#include <sol/sol.hpp>

namespace Engine
{

struct LuaScriptComponent
{
    std::string ScriptPath;
    sol::table Instance;
    Entity Entity;
};

} // namespace Engine