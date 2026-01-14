#pragma once

#include <glm/glm.hpp>
#include <iomanip>
#include <sstream>
#include <string>

namespace Engine
{

class Math
{
public:
    static std::string ToString(const glm::vec3 &vec, int precision = 2);
    static std::string ToString(const glm::vec4 &vec, int precision = 2);

    static bool DecomposeTransform(const glm::mat4 &transform, glm::vec3 &translation, glm::vec3 &rotation,
                                   glm::vec3 &scale);
};

} // namespace Engine