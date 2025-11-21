#pragma once

#include <Platform.hpp>

namespace Engine
{

enum class MaterialPropertyType
{
    None,
    Int,
    UInt,
    Float,
    Vec2,
    Vec3,
    Vec4,
    Texture
};

using MaterialPropertyValue = std::variant<int, unsigned int, float, glm::vec2, glm::vec3, glm::vec4>;

class MaterialProperty
{
public:
    MaterialProperty() : m_Type(MaterialPropertyType::None) {}
    // clang-format off
    template<typename T>
    // clang-format on
    MaterialProperty(const T &value) : m_Value(value)
    {
        if (std::holds_alternative<int>(m_Value))
            m_Type = MaterialPropertyType::Int;
        else if (std::holds_alternative<unsigned int>(m_Value))
            m_Type = MaterialPropertyType::UInt;
        else if (std::holds_alternative<float>(m_Value))
            m_Type = MaterialPropertyType::Float;
        else if (std::holds_alternative<glm::vec2>(m_Value))
            m_Type = MaterialPropertyType::Vec2;
        else if (std::holds_alternative<glm::vec3>(m_Value))
            m_Type = MaterialPropertyType::Vec3;
        else if (std::holds_alternative<glm::vec4>(m_Value))
            m_Type = MaterialPropertyType::Vec4;
        else
        {
            m_Type = MaterialPropertyType::None;
            LOG_ENGINE_WARN("MaterialProperty: Unsupported MaterialPropertyValue type");
        }
    }
    MaterialProperty(const std::shared_ptr<Texture2D> &texture)
        : m_Type(MaterialPropertyType::Texture), m_Texture(texture)
    {
    }

    // Operators
    operator bool() const { return m_Type != MaterialPropertyType::None; }

    // Getters
    MaterialPropertyType GetType() const { return m_Type; }
    const MaterialPropertyValue &GetValue() const { return m_Value; }
    std::shared_ptr<Texture2D> GetTexture() const { return m_Texture; }

    // Setters
    void SetValue(const MaterialPropertyValue &value) { m_Value = value; }
    void SetTexture(const std::shared_ptr<Texture2D> &texture) { m_Texture = texture; }

private:
    MaterialPropertyType m_Type;
    MaterialPropertyValue m_Value;
    std::shared_ptr<Texture2D> m_Texture;
};

} // namespace Engine
