#pragma once

#include "pch.hpp"

namespace MCEngine
{

struct Material
{
    glm::vec4 Color;
    float AmbientStrength;
    float DiffuseStrength;
    float SpecularStrength;
    float Shininess;

public:
    Material(const glm::vec4 &color, float ambient, float diffuse, float specular, float shininess);

public:
    std::string ToString() const;
    void Bind(const std::shared_ptr<Shader> &shader, const std::string &name) const;
};

} // namespace MCEngine
