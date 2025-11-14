#include "MaterialProperty.hpp"

MCEngine::MaterialProperty::MaterialProperty()
    : m_Type(MaterialPropertyType::None), m_Float(0.0f), m_Int(0), m_UInt(0), m_Vec2(0.0f), m_Vec3(0.0f),
      m_Vec4(0.0f), m_Mat4(1.0f), m_TextureSlot(0)
{
}

MCEngine::MaterialProperty::MaterialProperty(float value)
    : m_Type(MaterialPropertyType::Float), m_Float(value), m_Int(0), m_UInt(0), m_Vec2(0.0f), m_Vec3(0.0f),
      m_Vec4(0.0f), m_Mat4(1.0f), m_TextureSlot(0)
{
}

MCEngine::MaterialProperty::MaterialProperty(int value)
    : m_Type(MaterialPropertyType::Int), m_Float(0.0f), m_Int(value), m_UInt(0), m_Vec2(0.0f), m_Vec3(0.0f),
      m_Vec4(0.0f), m_Mat4(1.0f), m_TextureSlot(0)
{
}

MCEngine::MaterialProperty::MaterialProperty(unsigned int value)
    : m_Type(MaterialPropertyType::UInt), m_Float(0.0f), m_Int(0), m_UInt(value), m_Vec2(0.0f), m_Vec3(0.0f),
      m_Vec4(0.0f), m_Mat4(1.0f), m_TextureSlot(0)
{
}

MCEngine::MaterialProperty::MaterialProperty(const glm::vec2 &value)
    : m_Type(MaterialPropertyType::Vec2), m_Float(0.0f), m_Int(0), m_UInt(0), m_Vec2(value), m_Vec3(0.0f),
      m_Vec4(0.0f), m_Mat4(1.0f), m_TextureSlot(0)
{
}

MCEngine::MaterialProperty::MaterialProperty(const glm::vec3 &value)
    : m_Type(MaterialPropertyType::Vec3), m_Float(0.0f), m_Int(0), m_UInt(0), m_Vec2(0.0f), m_Vec3(value),
      m_Vec4(0.0f), m_Mat4(1.0f), m_TextureSlot(0)
{
}

MCEngine::MaterialProperty::MaterialProperty(const glm::vec4 &value)
    : m_Type(MaterialPropertyType::Vec4), m_Float(0.0f), m_Int(0), m_UInt(0), m_Vec2(0.0f), m_Vec3(0.0f),
      m_Vec4(value), m_Mat4(1.0f), m_TextureSlot(0)
{
}

MCEngine::MaterialProperty::MaterialProperty(const glm::mat4 &value)
    : m_Type(MaterialPropertyType::Mat4), m_Float(0.0f), m_Int(0), m_UInt(0), m_Vec2(0.0f), m_Vec3(0.0f),
      m_Vec4(0.0f), m_Mat4(value), m_TextureSlot(0)
{
}

MCEngine::MaterialProperty::MaterialProperty(const std::shared_ptr<Texture2D> &texture, unsigned int slot)
    : m_Type(MaterialPropertyType::Texture2D), m_Float(0.0f), m_Int(0), m_UInt(0), m_Vec2(0.0f), m_Vec3(0.0f),
      m_Vec4(0.0f), m_Mat4(1.0f), m_Texture2D(texture), m_TextureSlot(slot)
{
}

MCEngine::MaterialProperty::MaterialProperty(const MaterialProperty &other)
    : m_Type(other.m_Type), m_Texture2D(other.m_Texture2D), m_TextureSlot(other.m_TextureSlot)
{
    switch (m_Type)
    {
    case MaterialPropertyType::Float:
        m_Float = other.m_Float;
        break;
    case MaterialPropertyType::Int:
        m_Int = other.m_Int;
        break;
    case MaterialPropertyType::UInt:
        m_UInt = other.m_UInt;
        break;
    case MaterialPropertyType::Vec2:
        m_Vec2 = other.m_Vec2;
        break;
    case MaterialPropertyType::Vec3:
        m_Vec3 = other.m_Vec3;
        break;
    case MaterialPropertyType::Vec4:
        m_Vec4 = other.m_Vec4;
        break;
    case MaterialPropertyType::Mat4:
        m_Mat4 = other.m_Mat4;
        break;
    case MaterialPropertyType::Texture2D:
        // Already handled in initializer list
        break;
    default:
        break;
    }
}

MCEngine::MaterialProperty &MCEngine::MaterialProperty::operator=(const MaterialProperty &other)
{
    if (this == &other)
        return *this;

    m_Type = other.m_Type;
    m_Texture2D = other.m_Texture2D;
    m_TextureSlot = other.m_TextureSlot;

    switch (m_Type)
    {
    case MaterialPropertyType::Float:
        m_Float = other.m_Float;
        break;
    case MaterialPropertyType::Int:
        m_Int = other.m_Int;
        break;
    case MaterialPropertyType::UInt:
        m_UInt = other.m_UInt;
        break;
    case MaterialPropertyType::Vec2:
        m_Vec2 = other.m_Vec2;
        break;
    case MaterialPropertyType::Vec3:
        m_Vec3 = other.m_Vec3;
        break;
    case MaterialPropertyType::Vec4:
        m_Vec4 = other.m_Vec4;
        break;
    case MaterialPropertyType::Mat4:
        m_Mat4 = other.m_Mat4;
        break;
    case MaterialPropertyType::Texture2D:
        // Already handled above
        break;
    default:
        break;
    }

    return *this;
}

float MCEngine::MaterialProperty::GetFloat() const
{
    if (m_Type != MaterialPropertyType::Float)
    {
        LOG_ENGINE_WARN("MaterialProperty: Trying to get Float from non-Float property");
        return 0.0f;
    }
    return m_Float;
}

int MCEngine::MaterialProperty::GetInt() const
{
    if (m_Type != MaterialPropertyType::Int)
    {
        LOG_ENGINE_WARN("MaterialProperty: Trying to get Int from non-Int property");
        return 0;
    }
    return m_Int;
}

unsigned int MCEngine::MaterialProperty::GetUInt() const
{
    if (m_Type != MaterialPropertyType::UInt)
    {
        LOG_ENGINE_WARN("MaterialProperty: Trying to get UInt from non-UInt property");
        return 0;
    }
    return m_UInt;
}

glm::vec2 MCEngine::MaterialProperty::GetVec2() const
{
    if (m_Type != MaterialPropertyType::Vec2)
    {
        LOG_ENGINE_WARN("MaterialProperty: Trying to get Vec2 from non-Vec2 property");
        return glm::vec2(0.0f);
    }
    return m_Vec2;
}

glm::vec3 MCEngine::MaterialProperty::GetVec3() const
{
    if (m_Type != MaterialPropertyType::Vec3)
    {
        LOG_ENGINE_WARN("MaterialProperty: Trying to get Vec3 from non-Vec3 property");
        return glm::vec3(0.0f);
    }
    return m_Vec3;
}

glm::vec4 MCEngine::MaterialProperty::GetVec4() const
{
    if (m_Type != MaterialPropertyType::Vec4)
    {
        LOG_ENGINE_WARN("MaterialProperty: Trying to get Vec4 from non-Vec4 property");
        return glm::vec4(0.0f);
    }
    return m_Vec4;
}

glm::mat4 MCEngine::MaterialProperty::GetMat4() const
{
    if (m_Type != MaterialPropertyType::Mat4)
    {
        LOG_ENGINE_WARN("MaterialProperty: Trying to get Mat4 from non-Mat4 property");
        return glm::mat4(1.0f);
    }
    return m_Mat4;
}

std::shared_ptr<MCEngine::Texture2D> MCEngine::MaterialProperty::GetTexture2D() const
{
    if (m_Type != MaterialPropertyType::Texture2D)
    {
        LOG_ENGINE_WARN("MaterialProperty: Trying to get Texture2D from non-Texture2D property");
        return nullptr;
    }
    return m_Texture2D;
}

void MCEngine::MaterialProperty::SetFloat(float value)
{
    m_Type = MaterialPropertyType::Float;
    m_Float = value;
}

void MCEngine::MaterialProperty::SetInt(int value)
{
    m_Type = MaterialPropertyType::Int;
    m_Int = value;
}

void MCEngine::MaterialProperty::SetUInt(unsigned int value)
{
    m_Type = MaterialPropertyType::UInt;
    m_UInt = value;
}

void MCEngine::MaterialProperty::SetVec2(const glm::vec2 &value)
{
    m_Type = MaterialPropertyType::Vec2;
    m_Vec2 = value;
}

void MCEngine::MaterialProperty::SetVec3(const glm::vec3 &value)
{
    m_Type = MaterialPropertyType::Vec3;
    m_Vec3 = value;
}

void MCEngine::MaterialProperty::SetVec4(const glm::vec4 &value)
{
    m_Type = MaterialPropertyType::Vec4;
    m_Vec4 = value;
}

void MCEngine::MaterialProperty::SetMat4(const glm::mat4 &value)
{
    m_Type = MaterialPropertyType::Mat4;
    m_Mat4 = value;
}

void MCEngine::MaterialProperty::SetTexture2D(const std::shared_ptr<Texture2D> &texture, unsigned int slot)
{
    m_Type = MaterialPropertyType::Texture2D;
    m_Texture2D = texture;
    m_TextureSlot = slot;
}

std::string MCEngine::MaterialProperty::ToString() const
{
    switch (m_Type)
    {
    case MaterialPropertyType::Float:
        return "Float(" + std::to_string(m_Float) + ")";
    case MaterialPropertyType::Int:
        return "Int(" + std::to_string(m_Int) + ")";
    case MaterialPropertyType::UInt:
        return "UInt(" + std::to_string(m_UInt) + ")";
    case MaterialPropertyType::Vec2:
        return "Vec2(" + std::to_string(m_Vec2.x) + ", " + std::to_string(m_Vec2.y) + ")";
    case MaterialPropertyType::Vec3:
        return "Vec3(" + std::to_string(m_Vec3.x) + ", " + std::to_string(m_Vec3.y) + ", " + std::to_string(m_Vec3.z) +
               ")";
    case MaterialPropertyType::Vec4:
        return "Vec4(" + std::to_string(m_Vec4.x) + ", " + std::to_string(m_Vec4.y) + ", " + std::to_string(m_Vec4.z) +
               ", " + std::to_string(m_Vec4.w) + ")";
    case MaterialPropertyType::Mat4:
        return "Mat4";
    case MaterialPropertyType::Texture2D:
        return "Texture2D(slot: " + std::to_string(m_TextureSlot) + ")";
    default:
        return "None";
    }
}

