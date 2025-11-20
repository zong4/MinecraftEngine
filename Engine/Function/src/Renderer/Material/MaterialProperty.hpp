#pragma once

#include "pch.hpp"

namespace Engine
{

enum class MaterialPropertyType
{
    None,
    Float,
    Int,
    UInt,
    Vec2,
    Vec3,
    Vec4,
    Mat4,
    Texture2D
};

class MaterialProperty
{
public:
    MaterialProperty();
    MaterialProperty(float value);
    MaterialProperty(int value);
    MaterialProperty(unsigned int value);
    MaterialProperty(const glm::vec2 &value);
    MaterialProperty(const glm::vec3 &value);
    MaterialProperty(const glm::vec4 &value);
    MaterialProperty(const glm::mat4 &value);
    MaterialProperty(const std::shared_ptr<Texture2D> &texture, unsigned int slot = 0);

    // Copy constructor and assignment
    MaterialProperty(const MaterialProperty &other);
    MaterialProperty &operator=(const MaterialProperty &other);

    // Getters
    MaterialPropertyType GetType() const { return m_Type; }
    float GetFloat() const;
    int GetInt() const;
    unsigned int GetUInt() const;
    glm::vec2 GetVec2() const;
    glm::vec3 GetVec3() const;
    glm::vec4 GetVec4() const;
    glm::mat4 GetMat4() const;
    std::shared_ptr<Texture2D> GetTexture2D() const;
    unsigned int GetTextureSlot() const { return m_TextureSlot; }

    // Setters
    void SetFloat(float value);
    void SetInt(int value);
    void SetUInt(unsigned int value);
    void SetVec2(const glm::vec2 &value);
    void SetVec3(const glm::vec3 &value);
    void SetVec4(const glm::vec4 &value);
    void SetMat4(const glm::mat4 &value);
    void SetTexture2D(const std::shared_ptr<Texture2D> &texture, unsigned int slot = 0);

    // Utility
    std::string ToString() const;

private:
    MaterialPropertyType m_Type;

    // Use separate members instead of union for non-POD types (C++11 allows but requires careful handling)
    float m_Float;
    int m_Int;
    unsigned int m_UInt;
    glm::vec2 m_Vec2;
    glm::vec3 m_Vec3;
    glm::vec4 m_Vec4;
    glm::mat4 m_Mat4;

    std::shared_ptr<Texture2D> m_Texture2D;
    unsigned int m_TextureSlot;
};

} // namespace Engine
