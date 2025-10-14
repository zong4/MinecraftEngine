#pragma once

#include "pch.hpp"

namespace MCEngine
{

class Math
{
public:
    static bool DecomposeTransform(const glm::mat4 &transform, glm::vec3 &translation, glm::vec3 &rotation,
                                   glm::vec3 &scale);

    static std::string ToString(const glm::vec3 &vec, int precision = 2)
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(precision);
        oss << "{" << vec.x << ", " << vec.y << ", " << vec.z << "}";
        return oss.str();
    }

    static std::string ToString(const glm::vec4 &vec, int precision = 2)
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(precision);
        oss << "{" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << "}";
        return oss.str();
    }
};

} // namespace MCEngine